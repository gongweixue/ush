
#include "errno.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_sync.h"

typedef struct ush_hello_ack_s {
    pthread_cond_t      cond;
    pthread_condattr_t  condattr;
    pthread_mutex_t     mutex;
    ush_connect_t       conn;
    ush_bool_t          obsolete;
} * ush_sync_hello_ack_t;


static void
hello_ack_destroy(ush_sync_hello_ack_t *pAck) {
    ush_assert(pAck);
    if (!(*pAck)) {
        ush_log(LOG_LVL_ERROR, "ush_hello_ack_s NULL to be destroy");
        return;
    }

    if (1 != (*pAck)->obsolete) {
        ush_log(LOG_LVL_ERROR, "ack can not be destroy before obsolete");
        return;
    }

    pthread_mutex_destroy(&(*pAck)->mutex);
    pthread_condattr_destroy(&(*pAck)->condattr);
    pthread_cond_destroy(&(*pAck)->cond);

    ush_log(LOG_LVL_DETAIL, "free ack %p", pAck);
    free(*pAck);
    *pAck = NULL;

    return;
}

ush_ret_t
ush_sync_hello_ack_create(ush_sync_hello_ack_t *pAck, ush_connect_t conn) {
    ush_assert(pAck && conn);
    *pAck = NULL;

    ush_sync_hello_ack_t pMem =
        (ush_sync_hello_ack_t)malloc(sizeof(struct ush_hello_ack_s));
    if (!pMem) {
        ush_log(LOG_LVL_FATAL, "hello ack init:out of mem failed");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_DETAIL, "allocate mem for hello ack %p", pMem);

    ush_log(LOG_LVL_DETAIL, "init mutex and cond");
    if (0 != pthread_mutex_init(&pMem->mutex, NULL)) {
        ush_log(LOG_LVL_DETAIL, "free pMem %p", pMem);
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

        ush_log(LOG_LVL_DETAIL, "free pMem %p", pMem);
        free(pMem);
        ush_log(LOG_LVL_FATAL, "hello ack sync handle cond create failed");
        return USH_RET_FAILED;
    }

    pMem->conn = conn;
    pMem->obsolete = 0;

    *pAck = pMem;

    return USH_RET_OK;
}

ush_ret_t
ush_sync_hello_ack_wait_and_destroy(ush_sync_hello_ack_t  *pAck,
                                    const struct timespec *pDL) {
    ush_assert(pAck && *pAck);
    if (1 == (*pAck)->obsolete) {
        ush_log(LOG_LVL_FATAL, "can not wait a obosleted ack %p", *pAck);
        return USH_RET_WRONG_PARAM;
    }

    pthread_mutex_lock(&((*pAck)->mutex));

    int ret = USH_RET_OK;
    ush_log(LOG_LVL_DETAIL, "waiting the cond's signal, ack addr %p", (*pAck));
    int wait = 0;
    if (pDL) {
        wait = pthread_cond_timedwait(&((*pAck)->cond), &((*pAck)->mutex), pDL);
    } else {
        wait = pthread_cond_wait(&((*pAck)->cond), &((*pAck)->mutex));
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

    (*pAck)->obsolete = 1;

    ush_log(LOG_LVL_DETAIL, "wait return with ret code %d", ret);

    pthread_mutex_unlock(&((*pAck)->mutex));

    hello_ack_destroy(pAck);
    return ret;
}

ush_ret_t
ush_sync_hello_ack_signal(ush_sync_hello_ack_t *pAck,
                          ush_connidx_t         idx,
                          ush_cert_t            cert) {
    ush_assert(pAck && *pAck);

    ush_ret_t ret = USH_RET_OK;


    pthread_mutex_lock(&((*pAck)->mutex));

    if (1 == (*pAck)->obsolete) {
        ush_log(LOG_LVL_ERROR, "obsolete ack %p reached, destroy it", *pAck);
        goto BAILED;
    }

    ush_cert_t local_cert = USH_INVALID_CERT_VALUE;
    ret = ush_connect_get_cert((*pAck)->conn, &local_cert);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "connect cert get failed");
        goto BAILED;
    }
    if (cert != local_cert) { // nothing todo with this connection;
        ush_log(LOG_LVL_FATAL, "connect cert not match");
        ret = USH_RET_WRONG_PARAM;
        goto BAILED;
    }

    // set the ident from ushd back to the conn
    ret = ush_connect_set_connidx((*pAck)->conn, idx);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "connect remote idx can not set");
        goto BAILED;
    }

    pthread_cond_signal(&((*pAck)->cond));

BAILED:
    pthread_mutex_unlock(&((*pAck)->mutex));
    return ret;
}
