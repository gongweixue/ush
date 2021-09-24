#include "pthread.h"

#include "ush_assert.h"
#include "ush_comm_hello_msg.h"
#include "ush_comm_touch.h"
#include "ush_log.h"
#include "ush_type_pub.h"
#include "ush_time.h"

#include "ushd_sched_fifo.h"

////////////////////////////////////////////////////////////////////////////////
// @@@IMPORTANT !!!
// the impelementation should be considered on the performance perspective.
// maybe it need some optimization in the future due to the lock operation.
// or change to C++ with atomic operation and STL, or asm or intrisic-func
////////////////////////////////////////////////////////////////////////////////

// full queue sync
static pthread_mutex_t cs_full_q_locker  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cs_full_q_cond    = PTHREAD_COND_INITIALIZER;
static ush_ret_t cs_full_q_entry() {
    if (0 != pthread_mutex_lock(&cs_full_q_locker)) {
        ushd_log(LOG_LVL_ERROR, "entry cs_full_q_locker failed");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_DETAIL, "entry cs_full_q_locker");
    return USH_RET_OK;
}
static ush_ret_t cs_full_q_exit() {
    if (0 != pthread_mutex_unlock(&cs_full_q_locker)) {
        ushd_log(LOG_LVL_ERROR, "exit cs_full_q_locker failed");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_DETAIL, "exit cs_full_q_locker");
    return USH_RET_OK;
}
static ush_ret_t cs_full_q_wait() {
    ushd_log(LOG_LVL_DETAIL, "waiting for cs_full_q_cond");
    if (0 != pthread_cond_wait(&cs_full_q_cond, &cs_full_q_locker)) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}
static ush_ret_t cs_full_q_signal() {
    ushd_log(LOG_LVL_DETAIL, "trigger cs_full_q_cond");
    if (0 != pthread_cond_signal(&cs_full_q_cond)) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}

// empty queue sync
static pthread_mutex_t cs_empty_q_locker  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cs_empty_q_cond    = PTHREAD_COND_INITIALIZER;
static ush_ret_t cs_empty_q_entry() {
    if (0 != pthread_mutex_lock(&cs_empty_q_locker)) {
        ushd_log(LOG_LVL_ERROR, "entry cs_empty_q_locker failed");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_DETAIL, "entry cs_empty_q_locker");
    return USH_RET_OK;
}
static ush_ret_t cs_empty_q_exit() {
    if (0 != pthread_mutex_unlock(&cs_empty_q_locker)) {
        ushd_log(LOG_LVL_ERROR, "exit cs_empty_q_locker failed");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_DETAIL, "exit cs_empty_q_locker");
    return USH_RET_OK;
}
static ush_ret_t cs_empty_q_wait() {
    ushd_log(LOG_LVL_DETAIL, "waiting cs_empty_q_cond");
    if (0 != pthread_cond_wait(&cs_empty_q_cond, &cs_empty_q_locker)) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}
static ush_ret_t cs_empty_q_signal() {
    ushd_log(LOG_LVL_DETAIL, "trigger cs_empty_q_cond");
    if (0 != pthread_cond_signal(&cs_empty_q_cond)) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}


////////////////////////////////////////////////////////////////////////////////

//  how to work
//        -<----- retain empty, deal, release full ----<---
//        |                                               |
//        |     __________________________________        |
//        |     |                                 |       |
//        |  ---|-->       empty Q with 10elem  --|--->----
//        |  |  |_________________________________| <--cs_empty_q_locker
//        |  |
//        |  |__ retain full, deal, release empty ---<---
//        |                                             |
//        |     __________________________________      |
//        |     |                                 |     |
//        --->--|-->       full Q with 10elem   --|----->
//              |_________________________________| <--cs_full_q_locker
//
//
//  The empty&full just contains the idx(array offset) of sched_resource_type
//  Actually, the sched-fifo is fullQ, empty-Q is only the mempool for buffers
//  The empty/full queue is designed as a static @ring-buffer@ for convenience.
//
//  empty or full Queue:
//  ___________________________________________
//  | idx   idx                                |
//  |  0  |  2  |  5  |  7  |  1  |     |......|
//  |__________________________________________|
//     ^                             ^
//     |___ head                     |___ tail

//    sched_resource_type:
//    _______________________________________________________
//    |    |          |    |          |    |          |      |
//    |used|  buff... |used|  buff... |used|  buff... | x10  |
//    |____|__________|____|__________|____|__________|______|
//    ^                    ^
//    |____elem_array      |________sched_fifo_elem_type

#define FULL_ELEMENT    USHD_SCHED_FIFO_FULL
#define EMPTY_ELEMENT   USHD_SCHED_FIFO_EMPTY

#ifndef RESOURCE_COUNT
#define RESOURCE_COUNT    (100)
#endif
#define QUEUE_COUNT    (RESOURCE_COUNT + 1)  // tail never catchs head

typedef ush_s32_t fifo_idx_t;

/******************************************************************************/
/* mem pool */

typedef struct {
    ush_char_t buf[USH_COMM_TOUCH_Q_MSG_MAX_LEN];
    // ush_bool_t  used;
} sched_fifo_elem_type;

typedef struct {
    sched_fifo_elem_type elem_array[RESOURCE_COUNT];
} sched_resource_type;

static sched_resource_type resource;

static void
resource_init() {
    // memset and set the members to the initial-state.
    // not used as all the ADT are static type.
    return;
}


/******************************************************************************/
/* empty queue */
static fifo_idx_t empty_queue[QUEUE_COUNT];
static int empty_idx_head = 0;
static int empty_idx_tail = RESOURCE_COUNT;

static ush_char_t *
retain_elem_from_empty() {
    ush_char_t *ret = NULL;

    cs_empty_q_entry();

    if (empty_idx_head == empty_idx_tail) { // no buffer, wait...
        ushd_log(LOG_LVL_INFO, "no buffer, waiting......");
        cs_empty_q_wait();
    }

    // if head not catchs tail yet
    if (empty_idx_head != empty_idx_tail) {
        ret = resource.elem_array[empty_queue[empty_idx_head]].buf;
        empty_idx_head = (empty_idx_head + 1) % (QUEUE_COUNT);
        ushd_log(LOG_LVL_DETAIL, "dequeue buffer %p from empty", ret);
    } else {
        ushd_log(LOG_LVL_ERROR, "no empty buf to retain");
    }

    cs_empty_q_exit();

    return ret;
}
// no need to check the tail catch the head for 1producer and 1consumer.
static ush_ret_t
release_elem_to_empty(const ush_char_t *ptr) {
    ush_assert(ptr);
    ush_ret_t ret = USH_RET_FAILED;

    int idx = (ptr - resource.elem_array[0].buf) / sizeof(sched_fifo_elem_type);
    // confirm the prt is the addr of a buffer right there.
    if (ptr == resource.elem_array[idx].buf) {
        cs_empty_q_entry();
        empty_queue[empty_idx_tail] = idx;
        empty_idx_tail = (empty_idx_tail + 1) % (QUEUE_COUNT);
        ushd_log(LOG_LVL_DETAIL, "send signal to the blocking wait on emptyQ.");
        cs_empty_q_signal();
        cs_empty_q_exit();
        ret = USH_RET_OK;
        ushd_log(LOG_LVL_DETAIL, "release ptr %p to empty queue", ptr);
    } else {
        ushd_log(LOG_LVL_ERROR, "not a validate ptr %p", ptr);
    }

    return ret;
}


/* full queue */
static fifo_idx_t full_queue[QUEUE_COUNT];
static int full_idx_head = 0;
static int full_idx_tail = 0;

static ush_char_t *
retain_elem_from_full() {
    ush_char_t *ret = NULL;

    cs_full_q_entry();

    if (full_idx_head == full_idx_tail) { // no buffer, wait...
        ushd_log(LOG_LVL_INFO, "no buffer, waiting......");
        cs_full_q_wait();
    }

    ushd_log(LOG_LVL_DETAIL, "new data comming");

    // if head not catchs tail yet
    if (full_idx_head != full_idx_tail) {
        ret = resource.elem_array[full_queue[full_idx_head]].buf;
        full_idx_head = (full_idx_head + 1) % (QUEUE_COUNT);
        ushd_log(LOG_LVL_DETAIL, "dequeue buffer %p from full", ret);
    } else {
        ushd_log(LOG_LVL_ERROR, "no full buf to retain");
    }

    cs_full_q_exit();

    return ret;
}

static ush_ret_t
release_elem_to_full(const ush_char_t *ptr) {
    ush_assert(ptr);
    ush_ret_t ret = USH_RET_FAILED;

    int idx = (ptr - resource.elem_array[0].buf) / sizeof(sched_fifo_elem_type);
    // confirm the prt is the addr of a buffer right there.
    if (ptr == resource.elem_array[idx].buf) {
        cs_full_q_entry();
        full_queue[full_idx_tail] = idx;
        full_idx_tail = (full_idx_tail + 1) % (QUEUE_COUNT);
        ret = USH_RET_OK;
        ushd_log(LOG_LVL_INFO, "send signal to the blocking wait on fullQ.");
        cs_full_q_signal();
        cs_full_q_exit();
        ushd_log(LOG_LVL_DETAIL, "release ptr %p to full queue", ptr);
    } else {
        ushd_log(LOG_LVL_ERROR, "not a validate ptr %p", ptr);
    }

    return ret;
}

static void
queues_init() {
    // init empty queue's elements' idx in the resource
    for (int i = 0; i < RESOURCE_COUNT; ++i) {
        empty_queue[i] = i;
    }
    return;
}

/////////////////////////////////////////////////////////////
/////////////////////// public functions ////////////////////
/////////////////////////////////////////////////////////////

ush_ret_t
ushd_sched_fifo_init() {
    ushd_log(LOG_LVL_INFO, "init the sched fifo");
    resource_init();
    queues_init();
    return USH_RET_OK;
}

// always return USH_COMM_TOUCH_Q_MSG_MAX_LEN big buffer.
ush_char_t *
ushd_sched_fifo_retain(USHD_SCHED_FIFO_OPTION opt) {
    ush_char_t *ret = NULL;

    switch (opt) {
    case EMPTY_ELEMENT:
        ret = retain_elem_from_empty();
        break;
    case FULL_ELEMENT:
        ret = retain_elem_from_full();
        break;
    default:
        break;
    }

    return ret;
}

ush_ret_t
ushd_sched_fifo_release(ush_char_t *buf, USHD_SCHED_FIFO_OPTION opt) {
    if (!buf) {
        ushd_log(LOG_LVL_DETAIL, "release NULL to the fifo");
        return USH_RET_OK;
    }
    ush_ret_t ret = USH_RET_FAILED;

    switch (opt) {
    case EMPTY_ELEMENT:
        ret = release_elem_to_empty(buf);
        break;
    case FULL_ELEMENT:
        ret = release_elem_to_full(buf);
        break;
    default:
        break;
    }

    return ret;
}
