
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_comm_touch.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_touch.h"
#include "ush_listener.h"


typedef struct ush_connect {
    ush_connect_ident    ident;
    ush_touch_t          touch;
    ush_listener_t       listener;
    pthread_mutex_t      mutex;
} * ush_connect_t;


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
    ush_assert(conn);
    ush_u32_t idxRemote = getIdentIdx(conn->ident);

    ush_u32_t certify   = getIdentCertify(conn->ident);

    return ((-1 != idxRemote) && (-1 != certify));
}

ush_ret_t
ush_connect_create(ush_connect_t *pConn) {
    ush_assert(pConn);

    *pConn = NULL;

    ush_connect_t newMem = (ush_connect_t)malloc(sizeof(struct ush_connect));

    if (!newMem) {
        ush_log(LOG_LVL_FATAL, "connect memory allocation failed");
        return USH_RET_OUT_OF_MEM;
    }

    ush_ret_t ret = ush_touch_alloc(&(newMem->touch));
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "touch alloc failed");
        goto BAILED_CONN;
    }

    ret = ush_listener_alloc(&(newMem->listener));
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "listener alloc failed");
        goto BAILED_TOUCH;
    }

    if (0 != pthread_mutex_init(&(newMem->mutex), NULL)) { // init failed
        ush_log(LOG_LVL_FATAL, "mutex init of connect failed");
        ret = USH_RET_FAILED;
        goto BAILED_LISTENER;
    }

    ret = ush_touch_open(newMem->touch);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "open touch failed");
        goto BAILED_MUTEX;
    }

NORMAL:
    newMem->ident = CONNECT_IDENT_VALUE_DEFAULT; // means not initialed.
    *pConn = newMem;
    return USH_RET_OK;

BAILED_MUTEX:
    pthread_mutex_destroy(&(newMem->mutex));

BAILED_LISTENER:
    ush_listener_destroy_with_closing(&(newMem->listener));

BAILED_TOUCH:
    ush_touch_destroy_with_closing(&(newMem->touch));

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

    ush_touch_destroy_with_closing(&((*pConn)->touch));
    ush_listener_destroy_with_closing(&((*pConn)->listener));

    pthread_mutex_destroy(&((*pConn)->mutex));

    free(*pConn);

    *pConn = NULL;

    return USH_RET_OK;
}

ush_u32_t
ush_connect_generate_cert(const ush_char_t *seed) {
    ush_assert(seed);
    static ush_u32_t real_seed = 0;

    real_seed += *seed;

    srand(real_seed);
    ush_u32_t cert = rand();

    ush_log(LOG_LVL_INFO, "cert gen 0x%08x", cert);

    return cert;
}

ush_connect_ident
ush_connect_make_ident(ush_u32_t idx, ush_u32_t certify) {
    return (((ush_connect_ident)idx) << 32) | certify;
}

ush_ret_t
ush_connect_set_ident(ush_connect_t conn, ush_connect_ident ident) {
    ush_assert(conn);
    ush_ret_t ret = USH_RET_FAILED;

    if (!ush_connect_ident_valid(conn)) { // assign twice is not allow
        conn->ident = ident;
        ret          = USH_RET_OK;
    } else {
        ush_log(LOG_LVL_ERROR, "valid ident is already exist.");
    }

    return ret;
}

ush_ret_t
ush_connect_get_ident(ush_connect_t conn, ush_connect_ident *pIdent) {
    ush_assert(conn && pIdent);
    ush_ret_t ret = USH_RET_FAILED;
    if (ush_connect_ident_valid(conn)) {

        *pIdent = conn->ident;

        ret = USH_RET_OK;
    } else {
        ush_log(LOG_LVL_ERROR, "no valid ident here.");
    }
    return ret;
}

ush_ret_t
ush_connect_get_touch(ush_connect_t conn, ush_touch_t *ptr) {
    if (!conn || !ptr) {
        ush_log(LOG_LVL_FATAL, "connect or touch ptr NULL");
        return USH_RET_WRONG_PARAM;
    }

    *ptr = conn->touch;

    return USH_RET_OK;
}

ush_ret_t
ush_connect_cs_enter(ush_connect_t conn) {
    ush_assert(conn);
    // 0 for locked, others for not
    return !pthread_mutex_lock(&(conn->mutex)) ? USH_RET_OK : USH_RET_FAILED;
}

ush_ret_t
ush_connect_cs_exit(ush_connect_t conn) {
    ush_assert(conn);
    return !pthread_mutex_unlock(&(conn->mutex)) ? USH_RET_OK : USH_RET_FAILED;
}

ush_ret_t
ush_connect_listen_start(ush_connect_t conn, const ush_char_t *path) {
    ush_assert(conn && path);
    return USH_RET_OK;
}
