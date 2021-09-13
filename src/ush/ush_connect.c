#include "assert.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_comm_touch.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_touch.h"
#include "ush_listener.h"


typedef struct connect_t {
    ush_connect_ident    ident;
    ush_touch_t          touch;
    ush_listener_t       listener;
    pthread_mutex_t      mutex;
} *ush_connect_t;


static
ush_u32_t
getIdentIdx(const ush_connect_ident ident) {
    return (ush_u32_t)(ident >> 32); // high 32b
}

static
ush_u32_t
getIdentCertify(const ush_connect_ident ident) {
    return (ush_u32_t)(ident & 0xFFFFFFFF);         // low 32b
}

ush_bool_t
ush_connect_ident_valid(const ush_connect_t conn) {
    ush_u32_t idxRemote = getIdentIdx(conn->ident);
    ush_u32_t certify   = getIdentCertify(conn->ident);
    return ((-1 != idxRemote) && (-1 != certify));
}

ush_ret_t
ush_connect_alloc(ush_connect_t *pConn) {
    ush_connect_t tmp = (ush_connect_t)malloc(sizeof(struct connect_t));

    if (!tmp) {
        ush_log(USH_LOG_LVL_ERROR, "connect memory allocation failed\n");
        return USH_RET_OUT_OF_MEM;
    }

    tmp->ident = CONNECT_IDENT_VALUE_DEFAULT; // means not initialed.

    // pConn->touch = (ush_touch_t *)malloc(sizeof(ush_touch_t));
    ush_ret_t ret = ush_touch_alloc(&tmp->touch);
    if (USH_RET_OK != ret) {
        ush_log(USH_LOG_LVL_ERROR, "touch alloc failed\n");
        free(tmp);
        return ret;
    }

    // pConn->pListener = (ush_listener_t *)malloc(sizeof(ush_listener_t));
    ret = ush_listener_alloc(&tmp->listener);
    if (USH_RET_OK != ret) {
        ush_log(USH_LOG_LVL_ERROR, "listener alloc failed\n");
        free(tmp->touch);
        free(tmp);
        return ret;
    }

    *pConn = tmp;

    return USH_RET_OK;
}

ush_ret_t
ush_connect_init(ush_connect_t conn) {
    assert(conn);
    if (CONNECT_IDENT_VALUE_DEFAULT != conn->ident) { // already been initialed
        return USH_RET_WRONG_SEQ;
    }

    if (0 != pthread_mutex_init(&conn->mutex, NULL)) { // init failed
        ush_log(USH_LOG_LVL_ERROR, "mutex init of connect failed\n");
        free(conn);
        return USH_RET_FAILED;
    }

    // open touch
    ush_touch_t touch = NULL;
    ush_ret_t ret = ush_connect_get_touch(conn, &touch);
    if ((USH_RET_OK != ret) || (!touch)) {
        ush_log(USH_LOG_LVL_ERROR, "get touch failed\n");
        return ret;
    }
    ret = ush_touch_open(touch);
    if (USH_RET_OK != ret) {
        ush_log(USH_LOG_LVL_ERROR, "open touch failed\n");
        return ret;
    }

    return USH_RET_OK;
}

ush_ret_t
ush_connect_destroy(ush_connect_t conn) {
    if (!conn) {
        ush_log(USH_LOG_LVL_INFO, "connect NULL to be destroy\n");
        return USH_RET_OK;
    }

    // touch destroy with closing
    ush_touch_t touch = conn->touch;
    ush_touch_destroy_with_closing(touch);

    // listener destroy with closing
    ush_listener_t listener = conn->listener;
    ush_listener_destroy_with_closing(listener);

    pthread_mutex_destroy(&conn->mutex);

    free(conn);

    conn = NULL;

    return USH_RET_OK;
}

ush_connect_ident
ush_connect_make_ident(ush_u32_t idx, ush_u32_t certify) {
    return (((ush_connect_ident)idx) << 32) | certify;
}

ush_ret_t
ush_connect_set_ident(ush_connect_t conn, ush_connect_ident ident) {
    ush_ret_t ret = USH_RET_FAILED;
    pthread_mutex_lock(&conn->mutex);
    if (!ush_connect_ident_valid(conn)) { // assign twice is not allow
        conn->ident = ident;
        ret          = USH_RET_OK;
    }
    pthread_mutex_unlock(&conn->mutex);
    return ret;
}

ush_ret_t
ush_connect_get_ident(ush_connect_t conn, ush_connect_ident *pIdent) {
    ush_ret_t ret = USH_RET_FAILED;
    if (ush_connect_ident_valid(conn)) {
        pthread_mutex_lock(&conn->mutex);
        *pIdent = conn->ident;
        pthread_mutex_unlock(&conn->mutex);
        ret = USH_RET_OK;
    }
    return ret;
}

ush_ret_t ush_connect_get_touch(ush_connect_t conn, ush_touch_t *ptr) {
    if (!conn || !ptr) {
        return USH_RET_WRONG_PARAM;
    }
    pthread_mutex_lock(&conn->mutex);
    *ptr = conn->touch;
    pthread_mutex_unlock(&conn->mutex);
    return USH_RET_OK;
}

// ush_ret_t
// ush_connect_open_touch(ush_connect_t *pConn) {
//     if (!pConn) {
//         ush_log(ERR, "connection NULL to open failed\n");
//         return USH_RET_WRONG_PARAM;
//     }

//     ush_ret_t ret = USH_RET_OK;
//     ush_touch_t *ptr = NULL;
//     ret = ush_connect_get_touch(pConn, &ptr);
//     if ((USH_RET_OK != ret) || (!ptr)) {
//         ush_log(ERR, "open NULL pointer of touch failed\n");
//         return ret;
//     }
//     // pthread_mutex_lock(&pConn->mutex);
//     ret = ush_touch_open(ptr);
//     // pthread_mutex_unlock(&pConn->mutex);
//     return ret;
// }

// ush_ret_t
// ush_connect_close_touch(ush_connect_t *pConn) {
//     if (!pConn) {
//         ush_log(ERR, "close NULL pointer of connection failed\n");
//         return USH_RET_WRONG_PARAM;
//     }

//     ush_touch_t *ptr = NULL;
//     ush_ret_t ret = ush_connect_get_touch(pConn, &ptr);
//     if ((USH_RET_OK != ret) || (!ptr)) {
//         ush_log(ERR, "close NULL pointer of touch failed\n");
//         return ret;
//     }
//     // pthread_mutex_lock(&pConn->mutex);
//     ret = ush_touch_close(ptr);
//     // pthread_mutex_unlock(&pConn->mutex);
//     return ret;
// }
