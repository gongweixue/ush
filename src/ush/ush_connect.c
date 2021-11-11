
#include "pthread.h"
#include "string.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_comm_desc.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_random.h"
#include "ush_string.h"
#include "ush_sync.h"

#include "lstnr/ush_lstnr.h"
#include "ush_tch.h"
#include "ush_time.h"

#include "ush_realm.h"
#include "realm/ush_comm_realm.h"


typedef struct ush_connect_s {
    ush_cert_t           cert;
    ush_connidx_t        connidx;
    ush_tch_t            touch;
    ush_lstnr_t          listener;
    ush_realm_t          realm;
    ush_char_t           shortname_ts[USH_COMM_CONN_FULL_NAME_LEN_MAX];
    ush_u64_t            fingerprint;
} * ush_connect_t;

#define USH_INVALID_CONNECT_FINGERPRINT (0)

static ush_u64_t cal_fingerprint(const ush_connect_t conn) {
    ush_u64_t ret = 0;
    ret ^= (ush_u64_t)conn->cert;
    ret ^= (ush_u64_t)conn->touch;
    ret ^= (ush_u64_t)conn->listener;
    ret ^= (ush_u64_t)conn->realm;
    return ret;
}

static void
gen_name_ts(ush_char_t *name, ush_size_t sz, const ush_char_t *shortname) {
    if (strlen(shortname) >= sz) {
        ush_log(LOG_LVL_ERROR, "name too long, name gen failed");
        return;
    }

    // shortname-timestamp
    ush_char_t timestamp[16];
    ush_itoa(timestamp, time(NULL));
    if (strlen(shortname) + 1 + strlen(timestamp) >= sz) { // with an extra "-"
        ush_log(LOG_LVL_INFO, "name too long, but prefix-shortname gen.");
        return;
    }

    // name = shortname
    strncpy(name, timestamp, sz);
    strncat(name, "-", 1);
    strncat(name, shortname, USH_COMM_CONN_SHORTNAME_LEN_MAX);
}

ush_ret_t
ush_connect_create(ush_connect_t *pConn, const ush_char_t *name) {
    ush_assert(pConn && name);

    *pConn = NULL;

    ush_connect_t newMem = (ush_connect_t)malloc(sizeof(struct ush_connect_s));

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

    gen_name_ts(newMem->shortname_ts, sizeof(newMem->shortname_ts), name);
    ush_char_t fullname[USH_COMM_CONN_FULL_NAME_LEN_MAX];
    ush_cert_t cert = ush_random_generate_cert(newMem->shortname_ts);
    ush_string_gen_lstnr_fullname(fullname,
                                  sizeof(fullname),
                                  newMem->shortname_ts,
                                  cert);
    ret = ush_lstnr_open_start(&(newMem->listener), fullname);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "listener start failed");
        goto BAILED_TCH_DESTROY;
    }

    // register realm name
    ush_string_gen_realm_fullname(fullname,
                                  sizeof(fullname),
                                  newMem->shortname_ts,
                                  cert);
    ret = ush_realm_alloc(&(newMem->realm), fullname);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "realm alloc failed");
        goto BAILED_LSTNR_STOP_CLOSE;
    }


// NORMAL:
    newMem->cert = cert;
    newMem->connidx = USHD_INVALID_CONN_IDX_VALUE;
    newMem->fingerprint = cal_fingerprint(newMem);
    *pConn = newMem;
    return USH_RET_OK;

BAILED_LSTNR_STOP_CLOSE:
    ush_lstnr_stop_close(&(newMem->listener));

BAILED_TCH_DESTROY:
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

    ush_tch_destroy_with_closing(&((*pConn)->touch));
    ush_realm_destroy_with_closing(&((*pConn)->realm));
    ush_lstnr_stop_close(&((*pConn)->listener));

    free(*pConn);

    *pConn = NULL;

    return USH_RET_OK;
}

ush_ret_t
ush_connect_get_cert(ush_connect_t conn, ush_cert_t *ptr) {
    ush_assert(conn && ptr);
    if (!conn || !ptr) {
        *ptr = USH_INVALID_CERT_VALUE;
        return USH_RET_FAILED;
    }
    *ptr = conn->cert;

    return USH_RET_OK;
}

ush_ret_t
ush_connect_set_connidx(ush_connect_t conn, ush_connidx_t idx) {
    ush_assert(conn);
    if (!conn) {
        return USH_RET_WRONG_PARAM;
    }
    conn->connidx = idx;

    return USH_RET_OK;
}

ush_ret_t
ush_connect_get_connidx(const ush_connect_t conn, ush_connidx_t *ptr) {
    ush_assert(conn && ptr);
    if (!conn) {
        if (ptr) {*ptr = 0;}

        return USH_RET_WRONG_PARAM;
    }
    *ptr = conn->connidx;

    return USH_RET_OK;
}


static ush_ret_t // delay extra 500ms to avoid the NULL prt crush from sync
realize_timespec(struct timespec *ptr, ush_u16_t timeout) {
    if (!ptr) {
        ush_log(LOG_LVL_INFO, "timespec os null, just return OK.");
        return USH_RET_OK;
    }
    if (0 == timeout) {
        ush_log(LOG_LVL_ERROR, "timeout value invalid.");
        return USH_RET_WRONG_PARAM;
    }

    if (-1 == clock_gettime(CLOCK_MONOTONIC, ptr)) {
        ush_log(LOG_LVL_ERROR, "clock_gettime failed");
        return USH_RET_FAILED;
    }

    ptr->tv_sec += timeout;

    return USH_RET_OK;
}

ush_ret_t
ush_connect_link(ush_connect_t conn, ush_u16_t timeout) {
    ush_assert(conn);
    // param valid
    if (!conn) {
        ush_log(LOG_LVL_ERROR, "params NULL");
        return USH_RET_WRONG_PARAM;
    }

    ush_ret_t ret = USH_RET_OK;

    // use sync to wait feedback
    ush_connect_sync_t sync = NULL;
    ret = ush_connect_sync_create(&sync, conn);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "hello sync create failed");
        goto BAILED;
    }

    // for timeout
    struct timespec deadline;
    struct timespec *pDL = NULL;
    {
        if (0 != timeout) {
            ush_log(LOG_LVL_INFO, "need timeout operation");
            pDL = &deadline;
            ret = realize_timespec(pDL, timeout);
            if(USH_RET_OK != ret) {
                ush_log(LOG_LVL_ERROR, "realize timeout failed");
                ush_log(LOG_LVL_INFO, "ptr of deadline rollback to NULL");
                pDL = NULL; // disable the deadline
            }
        }
    }

    // prepare hello msg
    ush_comm_tch_hello_t msg = NULL;
    ush_cert_t cert = conn->cert;
    ret = ush_comm_tch_hello_create(&msg, conn->shortname_ts, sync, cert, pDL);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "hello create failed");
        goto DESTROY_SYNC;
    }

    ret = ush_connect_sync_lock(sync);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "sync locks failed");
        goto DESTROY_MSG;
    }

    // send with or without timeout
    ret = ush_tch_send_hello(conn->touch, msg, pDL);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "sending hello failed");
        goto UNLOCK_SYNC;
    }

    ret = ush_connect_sync_wait(sync, pDL);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "wait hello-sync failed, and return anyway");
        goto UNLOCK_SYNC;
    }

    ret = ush_realm_open(conn->realm);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "open realm failed");
        goto UNLOCK_SYNC;
    }

UNLOCK_SYNC:
    ush_connect_sync_unlock(sync);

DESTROY_MSG:
    ush_comm_tch_hello_destroy(&msg);

DESTROY_SYNC:
    ush_connect_sync_destroy(&sync);

BAILED:
    return ret;
}

ush_ret_t
ush_connect_send_to_realm(ush_connect_t conn, const ush_comm_d *msg) {
    if (!conn || !msg) {
        ushd_log(LOG_LVL_ERROR, "NULL ptr of params");
        return USH_RET_WRONG_PARAM;
    }

    if (USH_COMM_PORT_REALM != msg->port) {
        ushd_log(LOG_LVL_ERROR, "invalid msg sent by realm");
        return USH_RET_WRONG_PARAM;
    }

    if (!ush_connect_valid(conn)) {
        ushd_log(LOG_LVL_ERROR, "invalid connection");
        return USH_RET_WRONG_PARAM;
    }

    if (!conn->realm) {
        ushd_log(LOG_LVL_ERROR, "NULL ptr of realm");
        return USH_RET_WRONG_SEQ;
    }

    return ush_realm_send(conn->realm, (const ush_comm_realm_msg_d*)msg);
}


ush_bool_t
ush_connect_valid(const ush_connect_t conn) {
    if (!conn) {
        return USH_FALSE;
    }

    ush_bool_t ret =
        (conn->fingerprint == cal_fingerprint(conn)) ? USH_TRUE : USH_FALSE;

    return ret;
}

ush_ret_t
ush_connect_goodbye(ush_connect_t conn) {
    if (!conn) {
        return USH_RET_WRONG_PARAM;
    }

    if (!ush_connect_valid(conn)) {
        ush_log(LOG_LVL_ERROR, "connection is invalid");
        return USH_RET_WRONG_SEQ;
    }

    if (USH_INVALID_CONNECT_FINGERPRINT == conn->fingerprint) {
        ush_log(LOG_LVL_ERROR, "connection has been deprecated");
        return USH_RET_WRONG_SEQ;
    }

    // send goodbye msg
    ush_comm_tch_goodbye_t msg = NULL;
    ush_connidx_t          idx = conn->connidx;
    ush_cert_t            cert = conn->cert;
    if (USH_RET_OK != ush_comm_tch_goodbye_create(&msg, idx, cert)) {
        return USH_RET_OUT_OF_MEM;
    }

    ush_ret_t ret = ush_tch_send_goodbye(conn->touch, msg);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sending goodbye for conn %p failed", conn);
        return USH_RET_FAILED;
    }

    // deprecate this connection after sending goodbye msg
    conn->fingerprint = USH_INVALID_CONNECT_FINGERPRINT;

    // close realm queue
    ush_realm_destroy_with_closing(&(conn->realm));

    // close tch queue
    ush_tch_destroy_with_closing(&(conn->touch));

    // close&unlink lstnr queue & stop lstnr thread
    ush_lstnr_stop_close(&(conn->listener));

    return USH_RET_OK;
}
