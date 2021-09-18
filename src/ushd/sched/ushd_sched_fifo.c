#include "pthread.h"

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

pthread_mutex_t cs_full_q  = PTHREAD_MUTEX_INITIALIZER;
static ush_ret_t cs_full_q_entry() {
    if (0 != pthread_mutex_lock(&cs_full_q)) {
        ushd_log(LOG_LVL_ERROR, "entry cs_full_q failed");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_INFO, "entry cs_full_q");
    return USH_RET_OK;
}
static ush_ret_t cs_full_q_exit() {
    if (0 != pthread_mutex_unlock(&cs_full_q)) {
        ushd_log(LOG_LVL_ERROR, "exit cs_full_q failed");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_INFO, "exit cs_full_q");
    return USH_RET_OK;
}

pthread_mutex_t cs_empty_q  = PTHREAD_MUTEX_INITIALIZER;
static ush_ret_t cs_empty_q_entry() {
    if (0 != pthread_mutex_lock(&cs_empty_q)) {
        ushd_log(LOG_LVL_ERROR, "entry cs_empty_q failed");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_INFO, "entry cs_empty_q");
    return USH_RET_OK;
}
static ush_ret_t cs_empty_q_exit() {
    if (0 != pthread_mutex_unlock(&cs_empty_q)) {
        ushd_log(LOG_LVL_ERROR, "exit cs_empty_q failed");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_INFO, "exit cs_empty_q");
    return USH_RET_OK;
}


////////////////////////////////////////////////////////////////////////////////


#ifndef USH_SCHED_FIFO_COUNT
#define USH_SCHED_FIFO_COUNT    (10)
#endif

typedef struct {
    ush_char_t *pBuf;
    ush_bool_t  used;
} sched_fifo_elem_type;

typedef struct {
    ush_u32_t item_count;
    ush_u32_t buf_sz;
    sched_fifo_elem_type *first;
} sched_fifo_type;

static sched_fifo_type resource;


//  how to work
//        -<----- retain empty, deal, release full ----<---
//        |                                               |
//        |     __________________________________        |
//        |     |                                 |       |
//        |  ---|-->       empty Q with 10elem  --|--->----
//        |  |  |_________________________________| <--cs_empty_q
//        |  |
//        |  |__ retain full, deal, release empty ---<---
//        |                                             |
//        |     __________________________________      |
//        |     |                                 |     |
//        --->--|-->       full Q with 10elem   --|----->
//              |_________________________________| <--cs_full_q
//
//
//  The empty&full just contains the idx(array offset) of the sched_fifo_type
//  Actually, the sched fifo is fullQ, empty-Q is only the mempool for a buffer
//  The queue is designed as a static @ring-buffer@.
//  ___________________________________________
//  | idx   idx                                |
//  |  0  |  2  |  5  |  7  |  1  |  3  |......|
//  |__________________________________________|
//                  empty or full Queue
//

//    sched_fifo_type:
//    _______________________________________________________
//    |    |          |    |          |    |          |      |
//    |used|  buff... |used|  buff... |used|  buff... | x10  |
//    |____|__________|____|__________|____|__________|______|
//    ^                    ^
//    |____first           |________sched_fifo_elem_type




/////////////////////////////////////////////////////////////
/////////////////////// public functions ////////////////////
/////////////////////////////////////////////////////////////

ush_ret_t   ushd_sched_fifo_init() {

    return USH_RET_FAILED;
}

// always return USH_COMM_TOUCH_Q_MSG_MAX_LEN big buffer.
ush_char_t *ushd_sched_fifo_retain(USHD_SCHED_FIFO_OPTION type) {
    ush_time_delay_ms(500);
    return NULL;
}

ush_ret_t ushd_sched_fifo_release(ush_char_t *buf, USHD_SCHED_FIFO_OPTION opt) {
    ush_time_delay_ms(500);
    return USH_RET_FAILED;
}
