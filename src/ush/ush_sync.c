#include "assert.h"
#include "errno.h"
#include "pthread.h"
#include "stdlib.h"


#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_sync.h"


typedef struct ush_sync_hello_ack_t {
    pthread_cond_t     cond;
    pthread_condattr_t condattr;
    pthread_mutex_t    mutex;
    ush_connect_ident  connIdent;
    ush_pp_state_t     connState;
} ush_sync_hello_ack_t;


ush_ret_t
ush_sync_hello_ack_create(ush_sync_hello_ack_t *pAck) {
    void *pMem = malloc(sizeof(ush_sync_hello_ack_t));
    if (!pMem) {
        ush_log(USH_LOG_LVL_ERROR, "hello ack init:out of mem failed\n");
        return USH_RET_OUT_OF_MEM;
    }

    pAck = (ush_sync_hello_ack_t *)pMem;

    if (0 != pthread_mutex_init(&pAck->mutex, NULL)) {
        free(pMem);
        ush_log(USH_LOG_LVL_ERROR, "hello ack sync handle mutex create failed\n");
        return USH_RET_FAILED;
    }

    // TODO: need to judge the return value???
    pthread_condattr_init(&pAck->condattr);
    pthread_condattr_setclock(&pAck->condattr, CLOCK_MONOTONIC);

    if (0 != pthread_cond_init(&pAck->cond, &pAck->condattr)) {
        pthread_mutex_destroy(&pAck->mutex);
        pthread_condattr_destroy(&pAck->condattr);
        free(pMem);
        ush_log(USH_LOG_LVL_ERROR, "hello ack sync handle cond create failed\n");
        return USH_RET_FAILED;
    }

    pAck->connIdent = CONNECT_IDENT_VALUE_DEFAULT;
    pAck->connState = USH_PP_STATE_NOT_EXIST;

    return USH_RET_OK;
}

ush_ret_t
ush_sync_hello_ack_wait(ush_sync_hello_ack_t *pAck, const struct timespec *pDL) {
    assert(pAck);

    pthread_mutex_lock(&pAck->mutex);

    int ret = USH_RET_OK;
    switch (pthread_cond_timedwait(&pAck->cond, &pAck->mutex, pDL)) {
    case 0:
        break;
    case ETIMEDOUT:
        ush_log(USH_LOG_LVL_INFO, "cond wait timeout\n");
        ret = USH_RET_TIMEOUT;
        break;
    default :
        ush_log(USH_LOG_LVL_ERROR, "cond wait failed\n");
        ret = USH_RET_FAILED;
        break;
    }

    // TODO:may be something should be done here.
    // get ident to conn

    pthread_mutex_unlock(&pAck->mutex);
    return ret;
}

ush_ret_t
ush_sync_hello_ack_destroy(ush_sync_hello_ack_t *pAck) {
    assert(pAck);
    if (!pAck) {
        return USH_RET_OK;
    }
    pthread_mutex_destroy(&pAck->mutex);
    pthread_condattr_destroy(&pAck->condattr);
    pthread_cond_destroy(&pAck->cond);

    free(pAck);

    return USH_RET_OK;
}
