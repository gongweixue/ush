#ifndef USHD_THREAD_H
#define USHD_THREAD_H


#include "ush_type_pub.h"

typedef enum ushd_thread_tid_idx {
    USHD_THREAD_TID_IDX_TOUCH      = 0,
    USHD_THREAD_TID_IDX_SWCR       = 1,
    USHD_THREAD_TID_IDX_PUBLISHER  = 3,
    USHD_THREAD_TID_IDX_MAX
} ushd_thread_tid_idx_t;

typedef enum ushd_thread_state {
    USHD_THREAD_STATE_NON = 0,
    USHD_THREAD_STATE_RUNNING,
    USHD_THREAD_STATE_STOP
} ushd_thread_state_t;

ush_ret_t ushd_thread_set_tid(ushd_thread_tid_idx_t idx, pthread_t tid);
// ush_ret_t ushd_thread_get_tid(ushd_thread_tid_idx_t idx, pthread_t *ptr);
// ush_ret_t ushd_thread_stop(ushd_thread_tid_idx_t idx);
// ush_ret_t ushd_thread_state(ushd_thread_tid_idx_t idx,
//                                ushd_thread_state_t *ptr);

#endif // USHD_THREAD_H
