
#include "errno.h"
#include "fcntl.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#include "ush_assert.h"
#include "ush_comm_desc.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_random.h"
#include "ush_string.h"
#include "ush_sync.h"
#include "ush_tch.h"
#include "ush_type_pub.h"


typedef struct timespec timespec;

static ush_ret_t send_hello_and_wait(const ush_char_t      *pName,
                                     const timespec        *pDL,
                                     ush_connect_t          conn);

static ush_ret_t realize_timeout(timespec *ptr, ush_u16_t timeout);

static ush_ret_t get_info_from_hello_ack_cb(ush_sync_hello_ack_t ack);

static void gen_prefix_name(ush_char_t       *name,
                          ush_size_t        sz,
                          const ush_char_t *shortname);

ush_ret_t
ush_pipe_create(
    const ush_char_t *pName,
    USH_PIPE_MODE     mode,
    ush_u32_t         flag,
    ush_u16_t         timeout,
    ush_pvoid_t      *pParams,
    ush_size_t        paramCnt,
    ush_pipe_t       *pPipe)
{
    // params valid
    if (!pName || !pPipe || USH_PP_MODE_MAX_GUARD <= mode) {
        ush_log(LOG_LVL_FATAL, "wrong params for pipe create.");
        if (pPipe) {*pPipe = USH_INVALID_PIPE;}
        return USH_RET_WRONG_PARAM;
    }

    *pPipe = USH_INVALID_PIPE; // NULL for error return;

    if (USH_COMM_LSTNR_Q_SHORTNAME_LEN_MAX < strlen(pName)) {
        ush_log(LOG_LVL_FATAL, "name too long, limited to %d",
                USH_COMM_LSTNR_Q_SHORTNAME_LEN_MAX);
        return USH_RET_NOT_SUPPORT;
    }

    ush_char_t name[USH_COMM_CONN_NAME_LEN_MAX];
    gen_prefix_name(name, sizeof(name), pName);


    ush_ret_t ret = USH_RET_OK;


    // for timeout
    timespec deadline;
    timespec *pDL = NULL;
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

    ush_connect_t conn = NULL;
    ret = ush_connect_create(&conn, name);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "connect create failed");
        goto RET;
    }

    ret = send_hello_and_wait(name, pDL, conn);

    if (USH_RET_OK == ret) {
        *pPipe = (ush_u64_t)conn;
        ush_log(LOG_LVL_INFO, "set conn's ptr to handle returned");
    } else {
        ush_log(LOG_LVL_FATAL, "hello and hay failed");
    }

RET:
    return ret;
}

static ush_ret_t
send_hello_and_wait(const ush_char_t *pName,
                    const timespec   *pDL,
                    ush_connect_t     conn) {
    ush_assert(pName && conn);
    // param valid
    if (!pName || !conn) {
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
    int cert = USH_INVALID_CERT_VALUE;
    ush_connect_get_cert(conn, &cert);
    ush_comm_tch_hello_create(&hello, pName, ack, cert);

    // send with or without timeout
    ush_tch_t touch = NULL;
    ush_connect_get_tch(conn, &touch);
    ret = ush_tch_send_hello(touch, hello, pDL);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "sending hello failed");
    } else {
        ret = ush_sync_hello_ack_wait(ack, pDL, get_info_from_hello_ack_cb);
        if (USH_RET_OK != ret) {
            ush_log(LOG_LVL_ERROR, "wait hello-ack failed, and return anyway");

            ush_log(LOG_LVL_DETAIL, "destroy connect %p", conn);

            // Connect_t must be destroy before the ack reached, in case of the
            // ack addr reached from ushd after the listener thread cancled to
            // avoid the NULL ptr crushing.
            ush_connect_destroy(&conn);
        }
    }


    ush_log(LOG_LVL_DETAIL, "destroy hello ack and hello msg");
    ush_sync_hello_ack_destroy(&ack); // ack not needed any more.
    ush_comm_tch_hello_destroy(&hello);

    return ret;
}


static ush_ret_t
realize_timeout(timespec *ptr, ush_u16_t timeout) {
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

static ush_ret_t
get_info_from_hello_ack_cb(ush_sync_hello_ack_t ack) {
    ush_assert(ack);
    return USH_RET_OK;
}

static void
gen_prefix_name(ush_char_t *name, ush_size_t sz, const ush_char_t *shortname) {
    if (strlen(USH_COMM_LSTNR_Q_PATH_PREFIX) >= sz) {
        ush_log(LOG_LVL_ERROR, "name too long, name gen failed");
        return;
    }
    // prefix
    strcpy(name, USH_COMM_LSTNR_Q_PATH_PREFIX);

    if (strlen(name) + strlen(shortname) >= sz) {
        ush_log(LOG_LVL_INFO, "name too long, but only gen prefix.");
        return;
    }
    // prefix-shortname
    strcat(name, shortname);

    // prefix-shortname-timestamp
    ush_char_t timestamp[16];
    ush_itoa(timestamp, time(NULL));
    if (strlen(name) + 1 + strlen(timestamp) >= sz) { // with an extra "-"
        ush_log(LOG_LVL_INFO, "name too long, but prefix-shortname gen.");
        return;
    }
    strcat(name, "-");
    strcat(name, timestamp);
}
