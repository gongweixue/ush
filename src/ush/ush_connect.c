
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
#include "ush_tch.h"
#include "ush_realm.h"


typedef struct ush_connect {
    ush_cert_t           cert;
    ush_connidx_t        connidx;
    ush_tch_t            touch;
    ush_lstnr_t          listener;
    ush_realm_t          realm;
    pthread_mutex_t      mutex;
    ush_char_t           shortname_ts[USH_COMM_CONN_FULL_NAME_LEN_MAX];
} * ush_connect_t;


static ush_ret_t
connect_cs_entry(const ush_connect_t conn) {
    ush_assert(conn);
    ush_log(LOG_LVL_DETAIL, "entry cs of %p conn", conn);
    // 0 for locked, others for not
    return !pthread_mutex_lock(&(conn->mutex)) ? USH_RET_OK : USH_RET_FAILED;
}
static ush_ret_t
connect_cs_exit(const ush_connect_t conn) {
    ush_assert(conn);
    ush_log(LOG_LVL_DETAIL, "exit cs of %p conn", conn);
    return !pthread_mutex_unlock(&(conn->mutex)) ? USH_RET_OK : USH_RET_FAILED;
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
    strcpy(name, timestamp);
    strcat(name, "-");
    strcat(name, shortname);
}

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
        goto BAILED_MUTEX;
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
    ush_log(LOG_LVL_DETAIL, "connect create normal return, addr %p", *pConn);
    newMem->cert = cert;
    newMem->connidx = USHD_INVALID_CONN_IDX_VALUE;
    *pConn = newMem;
    return USH_RET_OK;

BAILED_LSTNR_STOP_CLOSE:
    ush_log(LOG_LVL_DETAIL, "destory mutext");
    ush_lstnr_stop_close(&(newMem->listener));

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

    connect_cs_entry(*pConn);
    ush_lstnr_stop_close(&((*pConn)->listener));

    ush_log(LOG_LVL_DETAIL, "destory touch/listener/mutext");
    ush_tch_destroy_with_closing(&((*pConn)->touch));

    connect_cs_exit(*pConn);
    pthread_mutex_destroy(&((*pConn)->mutex));

    ush_log(LOG_LVL_DETAIL, "and free conn mem %p", *pConn);
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

    connect_cs_entry(conn);
    *ptr = conn->cert;
    connect_cs_exit(conn);

    return USH_RET_OK;
}

ush_ret_t
ush_connect_set_connidx(ush_connect_t conn, ush_connidx_t idx) {
    ush_assert(conn);
    if (!conn) {
        return USH_RET_WRONG_PARAM;
    }

    connect_cs_entry(conn);
    conn->connidx = idx;
    connect_cs_exit(conn);

    return USH_RET_OK;
}

ush_ret_t
ush_connect_get_connidx(const ush_connect_t conn, ush_connidx_t *ptr) {
    ush_assert(conn && ptr);
    if (!conn) {
        if (ptr) {*ptr = 0;}

        return USH_RET_WRONG_PARAM;
    }

    connect_cs_entry(conn);
    *ptr = conn->connidx;
    connect_cs_exit(conn);

    return USH_RET_OK;
}

ush_ret_t
ush_connect_get_tch(ush_connect_t conn, ush_tch_t *ptr) {
    if (!conn || !ptr) {
        ush_log(LOG_LVL_FATAL, "connect or touch ptr NULL");
        return USH_RET_WRONG_PARAM;
    }

    connect_cs_entry(conn);
    *ptr = conn->touch;
    connect_cs_exit(conn);

    ush_log(LOG_LVL_DETAIL, "get touch successful, addr %p", *ptr);

    return USH_RET_OK;
}


static ush_ret_t
realize_timeout(struct timespec *ptr, ush_u16_t timeout) {
    if (!ptr) {
        ush_log(LOG_LVL_INFO, "timespec os null, just return OK.");
        return USH_RET_OK;
    }

    if (-1 == clock_gettime(CLOCK_MONOTONIC, ptr)) {
        ush_log(LOG_LVL_ERROR, "clock_gettime failed");
        return USH_RET_FAILED;
    }

    ush_log(LOG_LVL_DETAIL, "update the deadline");
    ptr->tv_sec += timeout + 1;

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

    // use ack to wait feedback
    ush_sync_hello_ack_t ack = NULL;
    ush_log(LOG_LVL_DETAIL, "create hello ack");
    ush_ret_t ret = ush_sync_hello_ack_create(&ack, conn);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "hello ack create failed");
        return ret;
    }

    // prepare hello msg
    ush_comm_tch_hello_t hello;
    ush_log(LOG_LVL_DETAIL, "create hello msg");
    ush_cert_t cert = USH_INVALID_CERT_VALUE;
    ush_connect_get_cert(conn, &cert);
    ush_comm_tch_hello_create(&hello, conn->shortname_ts, &ack, cert);

    // for timeout
    struct timespec deadline;
    struct timespec *pDL = NULL;
    if (0 != timeout) {
        ush_log(LOG_LVL_INFO, "need timeout operation");
        pDL = &deadline;
        ret = realize_timeout(pDL, timeout);
        if(USH_RET_OK != ret) {
            ush_log(LOG_LVL_ERROR, "realize timeout failed");
            ush_log(LOG_LVL_INFO, "ptr of deadline rollback to NULL");
            pDL = NULL; // disable the deadline
        }
    }
    ush_log(LOG_LVL_DETAIL, "timespec is %p", pDL);

    // send with or without timeout
    ush_tch_t touch = NULL;
    ush_connect_get_tch(conn, &touch);
    ret = ush_tch_send_hello(touch, hello, pDL);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "sending hello failed");
    } else {
        ret = ush_sync_hello_ack_wait(ack, pDL);
        if (USH_RET_OK != ret) {
            ush_log(LOG_LVL_ERROR, "wait hello-ack failed, and return anyway");

            ush_log(LOG_LVL_DETAIL, "destroy connect %p", conn);
        }
    }

    ush_log(LOG_LVL_DETAIL, "destroy hello ack and hello msg");
    ush_comm_tch_hello_destroy(&hello);

    return ret;
}
