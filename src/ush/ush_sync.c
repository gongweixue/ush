
#include "errno.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_connect.h"
#include "ush_define.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_sync.h"

typedef struct ush_connect_sync_s {
    pthread_cond_t      cond;
    pthread_condattr_t  condattr;
    pthread_mutex_t     mutex;
    ush_connect_t       conn;
} * ush_connect_sync_t;


ush_ret_t
ush_connect_sync_create(ush_connect_sync_t *pSync, ush_connect_t conn) {
    ush_assert(pSync && conn);
    *pSync = NULL;
    ush_ret_t ret = USH_RET_FAILED;

    ush_connect_sync_t pMem =
        (ush_connect_sync_t)malloc(sizeof(struct ush_connect_sync_s));
    if (!pMem) {
        ush_log(LOG_LVL_FATAL, "hello sync init:out of mem failed");
        ret = USH_RET_OUT_OF_MEM;
        goto RET;
    }

    if (0 != pthread_mutex_init(&pMem->mutex, NULL)) {
        ush_log(LOG_LVL_FATAL, "hello sync sync handle mutex create failed");
        ret = USH_RET_FAILED;
        goto FREE_MEM;
    }

    pthread_condattr_init(&pMem->condattr);
    pthread_condattr_setclock(&pMem->condattr, CLOCK_MONOTONIC);
    if (0 != pthread_cond_init(&pMem->cond, &pMem->condattr)) {
        ush_log(LOG_LVL_FATAL, "hello sync sync handle cond create failed");
        ret = USH_RET_FAILED;
        goto DESTROY_PTHREAD;
    }


    pMem->conn = conn;
    *pSync     = pMem;
    ret        = USH_RET_OK;
    goto RET;


DESTROY_PTHREAD:
    pthread_condattr_destroy(&pMem->condattr);
    pthread_mutex_destroy(&pMem->mutex);


FREE_MEM:
    free(pMem);

RET:
    return ret;
}


ush_ret_t
ush_connect_sync_destroy(ush_connect_sync_t *pSync) {
    ush_assert(pSync);
    if (NULL == (*pSync)) {
        ush_log(LOG_LVL_ERROR, "ush_connect_sync_s NULL to be destroy");
        return USH_RET_WRONG_PARAM;
    }

    pthread_mutex_destroy(&((*pSync)->mutex));
    pthread_condattr_destroy(&((*pSync)->condattr));
    pthread_cond_destroy(&((*pSync)->cond));

    free(*pSync);

    *pSync = NULL;

    return USH_RET_OK;
}

ush_ret_t
ush_connect_sync_lock(ush_connect_sync_t sync) {
    if (NULL == sync) {
        ush_log(LOG_LVL_ERROR, "ush_connect_sync_t NULL to be locked");
        return USH_RET_WRONG_PARAM;
    }
    int i = pthread_mutex_lock(&(sync->mutex));
    return (0 == i ? USH_RET_OK : USH_RET_FAILED);
}

ush_ret_t
ush_connect_sync_unlock(ush_connect_sync_t sync) {
    if (NULL == sync) {
        ush_log(LOG_LVL_ERROR, "ush_connect_sync_t NULL to be unlocked");
        return USH_RET_WRONG_PARAM;
    }
    int i = pthread_mutex_unlock(&(sync->mutex));
    return (0 == i ? USH_RET_OK : USH_RET_FAILED);
}

ush_ret_t
ush_connect_sync_wait(ush_connect_sync_t  sync, const struct timespec *pDL) {
    ush_assert(sync);

    int ret = USH_RET_OK;
    int wait = 0;
    if (pDL) {
        wait = pthread_cond_timedwait(&(sync->cond), &(sync->mutex), pDL);
        if (0 == wait) {
            ret = USH_RET_OK;
        } else if (ETIMEDOUT == wait) {
            ret = USH_RET_TIMEOUT;
        } else {
            ret = USH_RET_FAILED;
        }
    } else {
        wait = pthread_cond_wait(&(sync->cond), &(sync->mutex));
        ret = (0 == wait) ? USH_RET_OK : USH_RET_FAILED;
    }

    return ret;
}

ush_ret_t
ush_connect_sync_signal(ush_connect_sync_t sync,
                        ush_connidx_t      idx,
                        ush_cert_t         cert) {
    ush_assert(sync);

    ush_ret_t ret = USH_RET_OK;

    ush_cert_t local_cert = USH_INVALID_CERT_VALUE;
    ret = ush_connect_get_cert(sync->conn, &local_cert);
    if (USH_RET_OK != ret || cert != local_cert) {
        ush_log(LOG_LVL_FATAL, "connect cert verify failed");
        return USH_RET_WRONG_PARAM;
    }

    // set the ident from ushd back to the conn
    ret = ush_connect_set_connidx(sync->conn, idx);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "connect remote idx can not set");
        return USH_RET_WRONG_PARAM;
    }

    if (0 == pthread_cond_signal(&(sync->cond))) {
        ret = USH_RET_OK;
    } else {
        ret = USH_RET_FAILED;
    }

    return ret;
}
