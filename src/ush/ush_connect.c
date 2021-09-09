#include "mqueue.h"
#include "stdlib.h"
#include "pthread.h"

#include "ush_comm_touch.h"
#include "ush_connect.h"
#include "ush_pipe_pub.h"
#include "ush_log.h"
#include "ush_pipe_touch.h"


typedef struct listener_handle_t {
    mqd_t mq;
} listener_handle_t;


typedef struct connect_t {
    connect_ident     ident;
    // ush_pp_state_t    state;
    ush_pipe_touch_t  hTouch;
    listener_handle_t hListener;
    pthread_mutex_t   mutex;
} connect_t;


static
ush_u32_t
getIdentIdx(const connect_ident ident) {
    return (ush_u32_t)(ident >> 32); // high 32b
}

static
ush_u32_t
getIdentCertify(const connect_ident ident) {
    return (ush_u32_t)(ident & 0xFFFFFFFF);         // low 32b
}

ush_bool_t
connect_ident_valid(const connect_t *pConn) {
    ush_u32_t idxRemote = getIdentIdx(pConn->ident);
    ush_u32_t certify   = getIdentCertify(pConn->ident);
    return (-1 != idxRemote && -1 != certify);
}

ush_ret_t
connect_alloc(connect_t *pConn) {
    pConn = (connect_t *)malloc(sizeof(connect_t));
    if (!pConn) {
        ush_log(ERR, "connect memory allocation failed\n");
        return USH_RET_OUT_OF_MEM;
    }
    return USH_RET_OK;
}

ush_ret_t
connect_init(connect_t *pConn) {
    pConn->ident           = connect_make_ident(0xFFFFFFFF, 0xFFFFFFFF);// all 1
    // pConn->state           = USH_PP_STATE_NOT_EXIST;
    pConn->hTouch.mq       = -1;
    pConn->hListener.mq    = -1;
    if (0 != pthread_mutex_init(&pConn->mutex, NULL)) { // init failed
        ush_log(ERR, "mutex init of connect failed\n");
        free(pConn);
        return USH_RET_FAILED;
    }

    ush_ret_t ret = connect_open_touch(pConn);
    if (USH_RET_OK != ret) {
        return ret;
    }

    return USH_RET_OK;
}

ush_ret_t
connect_destroy(connect_t *pConn) {
    if (!pConn) {
        ush_log(MSG, "connect NULL to be destroy\n");
        return USH_RET_OK;
    }
    pthread_mutex_lock(&pConn->mutex);

    connect_close_touch(pConn);

    pthread_mutex_unlock(&pConn->mutex);

    pthread_mutex_destroy(&pConn->mutex);
    free(pConn);
    pConn = NULL;

    return USH_RET_OK;
}

connect_ident
connect_make_ident(ush_u32_t idx, ush_u32_t certify) {
    return (((connect_ident)idx) << 32) | certify;
}

ush_ret_t
connect_set_ident(connect_t *pConn, connect_ident ident) {
    ush_ret_t ret = USH_RET_FAILED;
    pthread_mutex_lock(&pConn->mutex);
    if (!connect_ident_valid(pConn)) { // assign twice is not allow
        pConn->ident = ident;
        ret          = USH_RET_OK;
    }
    pthread_mutex_unlock(&pConn->mutex);
    return ret;
}

ush_ret_t
connect_get_ident(connect_t *pConn, connect_ident *pIdent) {
    ush_ret_t ret = USH_RET_FAILED;
    if (connect_ident_valid(pConn)) {
        pthread_mutex_lock(&pConn->mutex);
        *pIdent = pConn->ident;
        pthread_mutex_unlock(&pConn->mutex);
        ret = USH_RET_OK;
    }
    return ret;
}

// ush_ret_t
// connect_set_state(connect_t *pConn, ush_pp_state_t state) {
//     pthread_mutex_lock(&pConn->mutex);
//     pConn->state = state;
//     pthread_mutex_unlock(&pConn->mutex);
//     return USH_RET_OK;
// }

// ush_ret_t
// connect_get_state(connect_t *pConn, ush_pp_state_t *pState) {
//     pthread_mutex_lock(&pConn->mutex);
//     *pState = pConn->state;
//     pthread_mutex_unlock(&pConn->mutex);
//     return USH_RET_OK;
// }

// ush_ret_t connect_set_touch(connect_t *pConn, const ush_pipe_touch_t *ptr) {
//     pthread_mutex_lock(&pConn->mutex);
//     pConn->hTouch = *ptr;
//     pthread_mutex_unlock(&pConn->mutex);
//     return USH_RET_OK;
// }

ush_ret_t connect_get_touch(connect_t *pConn, ush_pipe_touch_t *ptr) {
    pthread_mutex_lock(&pConn->mutex);
    *ptr = pConn->hTouch;
    pthread_mutex_unlock(&pConn->mutex);
    return USH_RET_OK;
}

// ush_ret_t connect_set_listener(connect_t *pConn, const listener_handle_t *ptr) {
//     pthread_mutex_lock(&pConn->mutex);
//     pConn->hListener = *ptr;
//     pthread_mutex_unlock(&pConn->mutex);
//     return USH_RET_OK;
// }

// ush_ret_t connect_get_listener(connect_t *pConn, listener_handle_t *ptr) {
//     pthread_mutex_lock(&pConn->mutex);
//     *ptr = pConn->hListener;
//     pthread_mutex_unlock(&pConn->mutex);
//     return USH_RET_OK;
// }

ush_ret_t
connect_open_touch(connect_t *pConn) {
    if (!pConn) {
        ush_log(ERR, "connection NULL to open failed\n");
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;
    pthread_mutex_lock(&pConn->mutex);

    if (-1 != pConn->hTouch.mq) { // maybe already opened
        ret = USH_RET_OK;
        goto BAILED;
    }

    pConn->hTouch.mq = mq_open(USH_COMM_TOUCH_Q_PATH, O_WRONLY);
    if (-1 == pConn->hTouch.mq) {
        ush_log(ERR, "touch open returns failed\n");
        ret = USH_RET_FAILED;
        goto BAILED;
    }

BAILED:
    pthread_mutex_unlock(&pConn->mutex);
    return ret;
}

ush_ret_t
connect_close_touch(connect_t *pConn) {
    if (!pConn) {
        ush_log(ERR, "close NULL pointer of touch failed\n");
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;
    pthread_mutex_lock(&pConn->mutex);
    if (-1 == pConn->hTouch.mq) { // already closed
        ret = USH_RET_OK;
        goto BAILED;
    }

    if (0 != mq_close(pConn->hTouch.mq)) {
        ush_log(USH_LOG_LVL_ERROR, "close touch queue failed\n");
        ret = USH_RET_FAILED;
        goto BAILED;
    }
    pConn->hTouch.mq = -1; // invalid value after close the queue.

BAILED:
    pthread_mutex_unlock(&pConn->mutex);
    return ret;
}
