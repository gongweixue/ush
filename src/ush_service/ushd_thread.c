#include "assert.h"
#include "pthread.h"

#include "ush_log.h"
#include "ush_type_pub.h"

#include "ushd_thread.h"


typedef struct ushd_thread_entity_t {
    ushd_thread_state_t state;
    pthread_t              tid;
} ushd_thread_entity_t;

static pthread_mutex_t sMutex4Threads = PTHREAD_MUTEX_INITIALIZER;
static ushd_thread_entity_t sThreads[USHD_THREAD_TID_IDX_MAX] = {
    {USHD_THREAD_STATE_NON, 0},
    {USHD_THREAD_STATE_NON, 0},
    {USHD_THREAD_STATE_NON, 0},
    {USHD_THREAD_STATE_NON, 0}
};

ush_ret_t ushd_thread_set_tid(ushd_thread_tid_idx_t idx, pthread_t tid) {
    // param valid
    assert(idx < USHD_THREAD_TID_IDX_MAX);
    if (idx >= USHD_THREAD_TID_IDX_MAX) {
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;
    if (0 == pthread_mutex_lock(&sMutex4Threads)) {
        if (USHd_THREAD_STATE_NON == sThreads[idx].state) {
            sThreads[idx].tid = tid;
            sThreads[idx].state = USHd_THREAD_STATE_RUNNING;
        } else {
            ret = USH_RET_WRONG_SEQ;
        }
        pthread_mutex_unlock(&sMutex4Threads);
    } else {
        ush_log(USH_LOG_LVL_ERROR, "set tid failed.\n");
        ret = USH_RET_FAILED;
    }

    return ret;
}

ush_ret_t ushd_thread_get_tid(ushd_thread_tid_idx_t idx, pthread_t *ptr) {
    // param valid
    assert(idx < USHd_THREAD_TID_IDX_MAX && ptr);
    if (idx >= USHd_THREAD_TID_IDX_MAX || !ptr) {
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;
    if (0 == pthread_mutex_lock(&sMutex4Threads)) {
        if (USHd_THREAD_STATE_RUNNING == sThreads[idx].state) {
            *ptr = sThreads[idx].tid;
        } else {
            ret = USH_RET_WRONG_SEQ;
        }
        pthread_mutex_unlock(&sMutex4Threads);
    } else {
        ush_log(USH_LOG_LVL_ERROR, "get tid failed.\n");
        ret = USH_RET_FAILED;
    }

    return ret;
}

ush_ret_t ushd_thread_stop(ushd_thread_tid_idx_t idx) {
    assert(idx < USHd_THREAD_TID_IDX_MAX);
    ush_ret_t ret = USH_RET_OK;
    // if (0 == pthread_mutex_lock(&sMutex4Threads)) {
    //     if (USHd_THREAD_STATE_RUNNING == sThreads[idx].state) {
    //         sThreads[idx].state = USHd_THREAD_STATE_STOP;
    //     } else {
    //         ret = USH_RET_WRONG_SEQ;
    //     }
    //     pthread_mutex_unlock(&sMutex4Threads);
    // } else {
    //     ush_log(USH_LOG_LVL_ERR, "thread stop failed.\n");
    //     ret = USH_RET_FAILED;
    // }

    return ret;
}

ush_ret_t ushd_thread_state(ushd_thread_tid_idx_t idx,
                               ushd_thread_state_t *ptr) {
    // param valid
    assert(idx < USHd_THREAD_TID_IDX_MAX && ptr);
    if (idx >= USHd_THREAD_TID_IDX_MAX || !ptr) {
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;
    if (0 == pthread_mutex_lock(&sMutex4Threads)) {
        *ptr = sThreads[idx].state;
        pthread_mutex_unlock(&sMutex4Threads);
    } else {
        ush_log(USH_LOG_LVL_ERROR, "get thread state failed.\n");
        ret = USH_RET_FAILED;
    }

    return ret;
}
