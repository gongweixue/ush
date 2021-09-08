#ifndef USH_SRV_THREAD_H
#define USH_SRV_THREAD_H

#include "pthread.h"
#include "ush_type.h"

typedef enum ush_srv_thread_tid_idx_t {
    USH_SRV_THREAD_TID_IDX_TOUCH      = 0,
    USH_SRV_THREAD_TID_IDX_SWCR       = 1,
    USH_SRV_THREAD_TID_IDX_PUBLISHER  = 3,
    USH_SRV_THREAD_TID_IDX_MAX
} ush_srv_thread_tid_idx_t;

typedef enum ush_srv_thread_state_t {
    USH_SRV_THREAD_STATE_NON = 0,
    USH_SRV_THREAD_STATE_RUNNING,
    USH_SRV_THREAD_STATE_STOP
} ush_srv_thread_state_t;

ush_ret_t ush_srv_thread_set_tid(ush_srv_thread_tid_idx_t idx, pthread_t tid);
// ush_ret_t ush_srv_thread_get_tid(ush_srv_thread_tid_idx_t idx, pthread_t *ptr);
// ush_ret_t ush_srv_thread_stop(ush_srv_thread_tid_idx_t idx);
// ush_ret_t ush_srv_thread_state(ush_srv_thread_tid_idx_t idx,
//                                ush_srv_thread_state_t *ptr);

#endif // USH_SRV_THREAD_H
