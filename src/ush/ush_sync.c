
#include "errno.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_sync.h"

typedef struct ush_hello_ack {
    pthread_cond_t      cond;
    pthread_condattr_t  condattr;
    pthread_mutex_t     mutex;
    ush_connect_ident   connIdentOnServer;
    ush_connect_t       connHdlOnClient;
} * ush_sync_hello_ack_t;


ush_ret_t
ush_sync_hello_ack_create(ush_sync_hello_ack_t *pAck, ush_connect_t conn) {
    ush_assert(pAck && conn);
    *pAck = NULL;

    ush_sync_hello_ack_t pMem =
        (ush_sync_hello_ack_t)malloc(sizeof(struct ush_hello_ack));
    if (!pMem) {
        ush_log(LOG_LVL_FATAL, "hello ack init:out of mem failed");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_DETAIL, "allocate mem for hello ack %p", pMem);

    ush_log(LOG_LVL_DETAIL, "init mutex and cond");
    if (0 != pthread_mutex_init(&pMem->mutex, NULL)) {
        free(pMem);
        ush_log(LOG_LVL_FATAL, "hello ack sync handle mutex create failed");
        return USH_RET_FAILED;
    }

    // TODO: need to judge the return value???
    pthread_condattr_init(&pMem->condattr);
    pthread_condattr_setclock(&pMem->condattr, CLOCK_MONOTONIC);

    if (0 != pthread_cond_init(&pMem->cond, &pMem->condattr)) {
        pthread_mutex_destroy(&pMem->mutex);
        pthread_condattr_destroy(&pMem->condattr);
        free(pMem);
        ush_log(LOG_LVL_FATAL, "hello ack sync handle cond create failed");
        return USH_RET_FAILED;
    }

    pMem->connIdentOnServer = CONNECT_IDENT_VALUE_DEFAULT;
    pMem->connHdlOnClient = conn;


    *pAck = pMem;

    return USH_RET_OK;
}

#define PTHREAD_COND_WAIT

ush_ret_t
ush_sync_hello_ack_wait(ush_sync_hello_ack_t         ack,
                        const struct timespec       *pDL,
                        ush_sync_hello_ack_wait_cb_t pCallback) {
    ush_assert(ack);

    pthread_mutex_lock(&ack->mutex);

    int ret = USH_RET_OK;
    ush_log(LOG_LVL_DETAIL, "waiting the cond's signal, ack addr %p", ack);
    int wait = 0;
    if (pDL) {
        wait = pthread_cond_timedwait(&ack->cond, &ack->mutex, pDL);
    } else {
        wait = pthread_cond_wait(&ack->cond, &ack->mutex);
    }
    switch (wait) {
    case 0:
        break;
    case ETIMEDOUT:
        ush_log(LOG_LVL_ERROR, "cond wait timeout");
        ret = USH_RET_TIMEOUT;
        break;
    default :
        ush_log(LOG_LVL_ERROR, "cond wait failed");
        ret = USH_RET_FAILED;
        break;
    }

    ush_log(LOG_LVL_DETAIL, "wait return with ret code %d", ret);

    // DO anythings you want once the ack arrived..
    if (USH_RET_OK == ret && pCallback) {
        ush_log(LOG_LVL_INFO, "ack-callback be called");
        ret = pCallback(ack);
    }

    pthread_mutex_unlock(&ack->mutex);
    return ret;
}

ush_ret_t
ush_sync_hello_ack_destroy(ush_sync_hello_ack_t *pAck) {
    ush_assert(pAck);
    if (!(*pAck)) {
        ush_log(LOG_LVL_INFO, "ush_hello_ack NULL to be destroy");
        return USH_RET_OK;
    }

    ush_log(LOG_LVL_DETAIL, "destroy mutex cond and free mem %p", *pAck);
    pthread_mutex_destroy(&(*pAck)->mutex);
    pthread_condattr_destroy(&(*pAck)->condattr);
    pthread_cond_destroy(&(*pAck)->cond);

    free(*pAck);
    *pAck = NULL;

    return USH_RET_OK;
}
