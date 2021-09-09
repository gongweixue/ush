#include "mqueue.h"
#include "stdlib.h"
#include "pthread.h"

#include "ush_comm_touch.h"
#include "ush_connection.h"
#include "ush_pipe_pub.h"
#include "ush_log.h"

typedef struct touch_handle_t {
    mqd_t mq;
} touch_handle_t;

typedef struct listener_handle_t {
    mqd_t mq;
} listener_handle_t;

typedef struct connection_ident {
    ush_s32_t         idxRemote;
    ush_s32_t         cert;
} connection_ident;

typedef struct connection_t {
    connection_ident  ident;
    ush_pp_state_t    state;
    touch_handle_t    hTouch;
    listener_handle_t hListener;
    pthread_mutex_t   mutex;
} connection_t;

ush_bool_t connection_ident_valid(const connection_t *pConn) {
    return (-1 != pConn->ident.idxRemote && -1 != pConn->ident.cert);
}

ush_ret_t connection_alloc(connection_t *pConn) {
    pConn = (connection_t *)malloc(sizeof(connection_t));
    if (!pConn) {
        return USH_RET_OUT_OF_MEM;
    }


    pConn->ident.idxRemote = -1;
    pConn->ident.cert      = -1;
    pConn->state     = USH_PP_STATE_NOT_EXIST;
    pConn->hTouch.mq    = -1;
    pConn->hListener.mq = -1;
    pthread_mutex_init(&pConn->mutex, NULL);

    return USH_RET_OK;
}

ush_ret_t connection_destroy(connection_t *pConn) {
    // // lock
    // close listener
    // close touch
    // // unlock
    // mutex destroy,

    // free (pConn)
}

ush_ret_t connection_set_ident(connection_t *pConn, const connection_ident *pIdent) {
    ush_ret_t ret = USH_RET_FAILED;
    pthread_mutex_lock(&pConn->mutex);
    if (!connection_ident_valid(pConn)) { // not init
        pConn->ident.idxRemote = pIdent->idxRemote;
        pConn->ident.cert      = pIdent->cert;
        ret                    = USH_RET_OK;
    }
    pthread_mutex_unlock(&pConn->mutex);
    return ret;
}

ush_ret_t connection_get_ident(connection_t *pConn, connection_ident *pIdent) {
    ush_ret_t ret = USH_RET_FAILED;
    if (connection_ident_valid(pConn)) {
        pthread_mutex_lock(&pConn->mutex);
        pIdent->idxRemote = pConn->ident.idxRemote;
        pIdent->idxRemote = pConn->ident.cert;
        pthread_mutex_unlock(&pConn->mutex);
        ret = USH_RET_OK;
    }
    return ret;
}

ush_ret_t connection_set_state(connection_t *pConn, ush_pp_state_t state) {
    pthread_mutex_lock(&pConn->mutex);
    pConn->state = state;
    pthread_mutex_unlock(&pConn->mutex);
    return USH_RET_OK;
}

ush_ret_t connection_get_state(connection_t *pConn, ush_pp_state_t *ptr) {
    pthread_mutex_lock(&pConn->mutex);
    *ptr = pConn->state;
    pthread_mutex_unlock(&pConn->mutex);
    return USH_RET_OK;
}

ush_ret_t connection_set_touch(connection_t *pConn, const touch_handle_t *ptr) {
    pthread_mutex_lock(&pConn->mutex);
    pConn->hTouch = *ptr;
    pthread_mutex_unlock(&pConn->mutex);
    return USH_RET_OK;
}

ush_ret_t connection_get_touch(connection_t *pConn, touch_handle_t *ptr) {
    pthread_mutex_lock(&pConn->mutex);
    *ptr = pConn->hTouch;
    pthread_mutex_unlock(&pConn->mutex);
    return USH_RET_OK;
}

ush_ret_t connection_set_listener(connection_t *pConn, const listener_handle_t *ptr) {
    pthread_mutex_lock(&pConn->mutex);
    pConn->hListener = *ptr;
    pthread_mutex_unlock(&pConn->mutex);
    return USH_RET_OK;
}

ush_ret_t connection_get_listener(connection_t *pConn, listener_handle_t *ptr) {
    pthread_mutex_lock(&pConn->mutex);
    *ptr = pConn->hListener;
    pthread_mutex_unlock(&pConn->mutex);
    return USH_RET_OK;
}

ush_ret_t connection_open_touch(connection_t *pConn) {
    if (!pConn) {
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;
    pthread_mutex_lock(&pConn->mutex);

    if (-1 != pConn->hTouch.mq) { // maybe already opened
        ret = USH_RET_WRONG_SEQ;
        goto BAILED;
    }

    pConn->hTouch.mq = mq_open(USH_COMM_TOUCH_Q_PATH, O_WRONLY);
    if (-1 == pConn->hTouch.mq) {
        ret = USH_RET_FAILED;
        goto BAILED;
    }

BAILED:
    pthread_mutex_unlock(&pConn->mutex);
    return USH_RET_OK;
}

ush_ret_t connection_close_touch(connection_t *pConn) {
    if (!pConn) {
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;
    pthread_mutex_lock(&pConn->mutex);
    if (-1 == pConn->hTouch.mq) { // already closed
        ret = USH_RET_WRONG_SEQ;
        goto BAILED;
    }

    if (mq_close(pConn->hTouch.mq)) {
        ush_log(USH_LOG_LVL_ERROR, "close touch queue failed\n");
        ret = USH_RET_FAILED;
        goto BAILED;
    }
    pConn->hTouch.mq = -1; // invalid value after close the queue.

BAILED:
    pthread_mutex_unlock(&pConn->mutex);
    return USH_RET_OK;
}
