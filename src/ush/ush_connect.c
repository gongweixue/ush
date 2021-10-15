
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_comm_desc.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_random.h"
#include "ush_string.h"
#include "ush_tch.h"


typedef struct ush_connect {
    ush_s32_t            cert;
    ush_s32_t            remote_idx;
    ush_tch_t            touch;
    ush_lstnr_t          listener;
    pthread_mutex_t      mutex;
} * ush_connect_t;

ush_ret_t
ush_connect_create(ush_connect_t *pConn, const ush_char_t *name) {
    ush_assert(pConn && name);

    *pConn = NULL;

    ush_connect_t newMem = (ush_connect_t)malloc(sizeof(struct ush_connect));

    if (!newMem) {
        ush_log(LOG_LVL_FATAL, "connect memory allocation failed");
        return USH_RET_OUT_OF_MEM;
    }

    ush_ret_t ret = ush_tch_alloc(&(newMem->touch));
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "tch alloc failed");
        goto BAILED_CONN;
    }

    ret = ush_tch_open(newMem->touch);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "open touch failed");
        goto BAILED_TCH_DESTROY;
    }

    if (0 != pthread_mutex_init(&(newMem->mutex), NULL)) { // init failed
        ush_log(LOG_LVL_FATAL, "mutex init of connect failed");
        ret = USH_RET_FAILED;
        goto BAILED_TCH_DESTROY;
    }

    ush_char_t certname[USH_COMM_CONN_NAME_LEN_MAX];
    ush_s32_t cert = ush_random_generate_cert(certname);
    ush_string_certname(certname, sizeof(certname), name, cert);

    ret = ush_lstnr_open_start(&(newMem->listener), certname);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "listener start failed");
        goto BAILED_MUTEX;
    }


// NORMAL:
    ush_log(LOG_LVL_DETAIL, "connect create normal return, addr %p", *pConn);
    newMem->cert = cert;
    newMem->remote_idx = USHD_INVALID_CONN_IDX_VALUE;
    *pConn = newMem;
    return USH_RET_OK;


BAILED_MUTEX:
    ush_log(LOG_LVL_DETAIL, "destory mutext");
    pthread_mutex_destroy(&(newMem->mutex));

BAILED_TCH_DESTROY:
    ush_log(LOG_LVL_DETAIL, "close & destory touch");
    ush_tch_destroy_with_closing(&(newMem->touch));

BAILED_CONN:
    free(newMem);

    return ret;
}

ush_ret_t
ush_connect_destroy(ush_connect_t *pConn) {
    ush_assert(pConn);
    if (!(*pConn)) {
        ush_log(LOG_LVL_INFO, "connect_t NULL to be destroy");
        return USH_RET_OK;
    }

    ush_lstnr_stop_close(&((*pConn)->listener));

    ush_log(LOG_LVL_DETAIL, "destory touch/listener/mutext");
    ush_tch_destroy_with_closing(&((*pConn)->touch));

    pthread_mutex_destroy(&((*pConn)->mutex));

    ush_log(LOG_LVL_DETAIL, "and free conn mem %p", *pConn);
    free(*pConn);

    *pConn = NULL;

    return USH_RET_OK;
}

ush_ret_t
ush_connect_get_cert(ush_connect_t conn, ush_s32_t *ptr) {
    ush_assert(conn && ptr);
    if (!conn || !ptr) {
        *ptr = USH_INVALID_CERT_VALUE;
        return USH_RET_FAILED;
    }

    *ptr = conn->cert;

    return USH_RET_OK;
}

ush_ret_t
ush_connect_set_remote_idx(ush_connect_t conn, ush_s32_t idx) {
    ush_assert(conn);
    if (!conn) {
        return USH_RET_WRONG_PARAM;
    }

    conn->remote_idx = idx;
    return USH_RET_OK;
}

ush_ret_t
ush_connect_get_remote_idx(const ush_connect_t conn, ush_s32_t *ptr) {
    ush_assert(conn && ptr);
    if (!conn) {
        if (ptr) {*ptr = 0;}

        return USH_RET_WRONG_PARAM;
    }

    *ptr = conn->remote_idx;
    return USH_RET_OK;
}

ush_ret_t
ush_connect_get_tch(ush_connect_t conn, ush_tch_t *ptr) {
    if (!conn || !ptr) {
        ush_log(LOG_LVL_FATAL, "connect or touch ptr NULL");
        return USH_RET_WRONG_PARAM;
    }

    *ptr = conn->touch;
    ush_log(LOG_LVL_DETAIL, "get touch successful, addr %p", *ptr);

    return USH_RET_OK;
}

ush_ret_t
ush_connect_cs_enter(ush_connect_t conn) {
    ush_assert(conn);
    ush_log(LOG_LVL_DETAIL, "enter cs of %p conn", conn);
    // 0 for locked, others for not
    return !pthread_mutex_lock(&(conn->mutex)) ? USH_RET_OK : USH_RET_FAILED;
}

ush_ret_t
ush_connect_cs_exit(ush_connect_t conn) {
    ush_assert(conn);
    ush_log(LOG_LVL_DETAIL, "exit cs of %p conn", conn);
    return !pthread_mutex_unlock(&(conn->mutex)) ? USH_RET_OK : USH_RET_FAILED;
}
