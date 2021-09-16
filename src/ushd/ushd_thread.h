#ifndef USHD_THREAD_H
#define USHD_THREAD_H


#include "ush_type_pub.h"

typedef enum USHD_THREAD_TID_IDX {
    USHD_THREAD_TID_IDX_TOUCH      = 0,
    USHD_THREAD_TID_IDX_SWCR       = 1,
    USHD_THREAD_TID_IDX_PUBLISHER  = 3,
    USHD_THREAD_TID_IDX_MAX
} USHD_THREAD_TID_IDX;

typedef enum USHD_THREAD_STATE {
    USHD_THREAD_STATE_NON = 0,
    USHD_THREAD_STATE_RUNNING,
    USHD_THREAD_STATE_STOP
} USHD_THREAD_STATE;

ush_ret_t ushd_thread_set_tid(USHD_THREAD_TID_IDX idx, pthread_t tid);
// ush_ret_t ushd_thread_get_tid(USHD_THREAD_TID_IDX idx, pthread_t *ptr);
// ush_ret_t ushd_thread_stop(USHD_THREAD_TID_IDX idx);
// ush_ret_t USHD_THREAD_STATE(USHD_THREAD_TID_IDX idx,
//                                USHD_THREAD_STATE *ptr);

#endif // USHD_THREAD_H
