#include "assert.h"
#include "pthread.h"
#include "ush_log.h"
#include "ush_srv_thread.h"
#include "ush_type_pub.h"


typedef struct ush_srv_thread_entity_t {
    ush_srv_thread_state_t state;
    pthread_t              tid;
} ush_srv_thread_entity_t;

static pthread_mutex_t sMutex4Threads = PTHREAD_MUTEX_INITIALIZER;
static ush_srv_thread_entity_t sThreads[USH_SRV_THREAD_TID_IDX_MAX] = {
    {USH_SRV_THREAD_STATE_NON, 0},
    {USH_SRV_THREAD_STATE_NON, 0},
    {USH_SRV_THREAD_STATE_NON, 0},
    {USH_SRV_THREAD_STATE_NON, 0}
};

ush_ret_t ush_srv_thread_set_tid(ush_srv_thread_tid_idx_t idx, pthread_t tid) {
    // param valid
    assert(idx < USH_SRV_THREAD_TID_IDX_MAX);
    if (idx >= USH_SRV_THREAD_TID_IDX_MAX) {
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;
    if (0 == pthread_mutex_lock(&sMutex4Threads)) {
        if (USH_SRV_THREAD_STATE_NON == sThreads[idx].state) {
            sThreads[idx].tid = tid;
            sThreads[idx].state = USH_SRV_THREAD_STATE_RUNNING;
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

ush_ret_t ush_srv_thread_get_tid(ush_srv_thread_tid_idx_t idx, pthread_t *ptr) {
    // param valid
    assert(idx < USH_SRV_THREAD_TID_IDX_MAX && ptr);
    if (idx >= USH_SRV_THREAD_TID_IDX_MAX || !ptr) {
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;
    if (0 == pthread_mutex_lock(&sMutex4Threads)) {
        if (USH_SRV_THREAD_STATE_RUNNING == sThreads[idx].state) {
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

ush_ret_t ush_srv_thread_stop(ush_srv_thread_tid_idx_t idx) {
    assert(idx < USH_SRV_THREAD_TID_IDX_MAX);
    ush_ret_t ret = USH_RET_OK;
    // if (0 == pthread_mutex_lock(&sMutex4Threads)) {
    //     if (USH_SRV_THREAD_STATE_RUNNING == sThreads[idx].state) {
    //         sThreads[idx].state = USH_SRV_THREAD_STATE_STOP;
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

ush_ret_t ush_srv_thread_state(ush_srv_thread_tid_idx_t idx,
                               ush_srv_thread_state_t *ptr) {
    // param valid
    assert(idx < USH_SRV_THREAD_TID_IDX_MAX && ptr);
    if (idx >= USH_SRV_THREAD_TID_IDX_MAX || !ptr) {
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
