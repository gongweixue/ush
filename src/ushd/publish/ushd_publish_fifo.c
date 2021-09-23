#include "pthread.h"
#include "stdlib.h"

#include "ush_log.h"
#include "ush_type_pub.h"

#include "ushd_publish_fifo.h"

#define FIFO_MSG_MAX_COUNT    (64)

typedef struct fifo_msg {
    ush_char_t data[USHD_PUBLISH_FIFO_MSG_MAX_SIZE];
} fifo_msg;

typedef struct publish_fifo {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    fifo_msg        buffer[FIFO_MSG_MAX_COUNT];
    ush_s32_t       head;
    ush_s32_t       tail;
} * ushd_publish_fifo_t;

ushd_publish_fifo_t ushd_publish_fifo_create() {
    ushd_publish_fifo_t fifo =
        (ushd_publish_fifo_t)malloc(sizeof(struct publish_fifo));
    if (!fifo) {
        goto RET;
    }

    if (0 != pthread_mutex_init(&fifo->mutex, NULL)) {
        goto BAILED_FIFO;
    }

    if (0 != pthread_cond_init(&fifo->cond, NULL)) {
        goto BAILED_MUTEX;
    }

    fifo->head = 0;
    fifo->tail = 0;
    goto RET;

BAILED_MUTEX:
    pthread_mutex_destroy(&fifo->mutex);

BAILED_FIFO:
    free(fifo);
    fifo = NULL;

RET:
    return fifo;

}




ush_ret_t
ushd_publish_fifo_push(ushd_publish_fifo_t fifo,
                       publish_fifo_msg_desc *pmsg,
                       ush_size_t sz) {

}








// #ifndef RESOURCE_COUNT
// #define RESOURCE_COUNT    (100)
// #endif
// #define QUEUE_COUNT    (RESOURCE_COUNT + 2)  // tail never catchs head

// typedef ush_s32_t fifo_idx_t;


// // full queue sync
// static pthread_mutex_t cs_full_q_locker  = PTHREAD_MUTEX_INITIALIZER;
// static pthread_cond_t  cs_full_q_cond    = PTHREAD_COND_INITIALIZER;
// static ush_ret_t cs_full_q_entry() {
//     if (0 != pthread_mutex_lock(&cs_full_q_locker)) {
//         ushd_log(LOG_LVL_ERROR, "entry cs_full_q_locker failed");
//         return USH_RET_FAILED;
//     }
//     ushd_log(LOG_LVL_DETAIL, "entry cs_full_q_locker");
//     return USH_RET_OK;
// }
// static ush_ret_t cs_full_q_exit() {
//     if (0 != pthread_mutex_unlock(&cs_full_q_locker)) {
//         ushd_log(LOG_LVL_ERROR, "exit cs_full_q_locker failed");
//         return USH_RET_FAILED;
//     }
//     ushd_log(LOG_LVL_DETAIL, "exit cs_full_q_locker");
//     return USH_RET_OK;
// }
// static ush_ret_t cs_full_q_wait() {
//     if (0 != pthread_cond_wait(&cs_full_q_cond, &cs_full_q_locker)) {
//         return USH_RET_FAILED;
//     }
//     return USH_RET_OK;
// }
// static ush_ret_t cs_full_q_signal() {
//     if (0 != pthread_cond_signal(&cs_full_q_cond)) {
//         return USH_RET_FAILED;
//     }
//     return USH_RET_OK;
// }


// /* full queue */
// static fifo_idx_t full_queue[QUEUE_COUNT];
// static int full_idx_head = 0;
// static int full_idx_tail = 0;

// static ush_char_t *
// retain_elem_from_full() {
//     ush_char_t *ret = NULL;

//     cs_full_q_entry();

//     if (full_idx_head == full_idx_tail) { // no buffer, wait...
//         ushd_log(LOG_LVL_INFO, "no buffer, waiting......");
//         cs_full_q_wait();
//     }

//     // if head not catchs tail yet
//     if (full_idx_head != full_idx_tail) {
//         ret = resource.elem_array[full_queue[full_idx_head]].buf;
//         full_idx_head = (full_idx_head + 1) % (QUEUE_COUNT);
//         ushd_log(LOG_LVL_DETAIL, "dequeue buffer %p from full", ret);
//     } else {
//         ushd_log(LOG_LVL_ERROR, "no full buf to retain");
//     }

//     cs_full_q_exit();

//     return ret;
// }

// static ush_ret_t
// release_elem_to_full(const ush_char_t *ptr) {
//     ush_assert(ptr);
//     ush_ret_t ret = USH_RET_FAILED;

//     int idx = (ptr - resource.elem_array[0].buf) / sizeof(sched_fifo_elem_type);
//     // confirm the prt is the addr of a buffer right there.
//     if (ptr == resource.elem_array[idx].buf) {
//         cs_full_q_entry();
//         full_queue[full_idx_tail] = idx;
//         full_idx_tail = (full_idx_tail + 1) % (QUEUE_COUNT);
//         ret = USH_RET_OK;
//         ushd_log(LOG_LVL_INFO, "send signal to the blocking wait on fullQ.");
//         cs_full_q_signal();
//         cs_full_q_exit();
//         ushd_log(LOG_LVL_DETAIL, "release ptr %p to full queue", ptr);
//     } else {
//         ushd_log(LOG_LVL_ERROR, "not a validate ptr %p", ptr);
//     }

//     return ret;
// }
