
#include "errno.h"
#include "fcntl.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#include "ush_assert.h"
#include "ush_comm_hello_msg.h"
#include "ush_comm_touch.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_sync.h"
#include "ush_touch.h"
#include "ush_type_pub.h"


typedef struct timespec timespec;

static ush_ret_t send_hello_and_wait(const ush_char_t      *pName,
                                const timespec        *pDL,
                                ush_connect_t          conn);

static ush_ret_t realize_timeout(timespec *ptr, ush_u16_t timeout);

static ush_ret_t get_info_from_hello_ack_cb(ush_sync_hello_ack_t ack);

ush_ret_t
ush_pipe_create(
    const ush_char_t *pName,
    USH_PIPE_MODE     mode,
    ush_u32_t         flag,
    ush_u16_t         timeout,
    ush_vptr_t       *pParams,
    ush_size_t        paramCnt,
    ush_pp_hdl_t     *pHdl)
{
    // params valid
    if (!pName || !pHdl || USH_PP_MODE_MAX_GUARD <= mode) {
        ush_log(LOG_LVL_FATAL, "wrong params for pipe create.");
        return USH_RET_WRONG_PARAM;
    }
    ush_assert(strlen(pName) < USH_COMM_HELLO_MSG_NAME_LEN_MAX);
    ush_assert(strlen(pName) >= USH_COMM_HELLO_MSG_NAME_LEN_MIN);

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
    ret = ush_connect_create(&conn, pName);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "connect create failed");
        goto RET;
    }

    ret = send_hello_and_wait(pName, pDL, conn);

    if (USH_RET_OK == ret) {
        *pHdl = (ush_s64_t)conn;
        ush_log(LOG_LVL_INFO, "set conn's ptr to handle returned");
    } else {
        *pHdl = -1;
        ush_log(LOG_LVL_FATAL, "hello and howareyou failed");
        ush_log(LOG_LVL_DETAIL, "destroy connect %p", conn);
        ush_connect_destroy(&conn);
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
    ush_comm_hello_msg_t hello;
    ush_log(LOG_LVL_DETAIL, "create hello msg");
    ush_s32_t cert = ush_connect_generate_cert(pName);
    ush_comm_hello_msg_create(&hello, pName, ack, cert);
    ush_comm_hello_msg_testpoint(hello);

    // send with or without timeout
    ush_touch_t touch = NULL;
    ush_connect_get_touch(conn, &touch);
    ret = ush_touch_send_hello(touch, hello, pDL);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "sending hello failed");
    } else {
        ret = ush_sync_hello_ack_wait(ack, pDL, get_info_from_hello_ack_cb);
        if (USH_RET_OK != ret) {
            ush_log(LOG_LVL_ERROR, "wait hello-ack failed, and return anyway");
        }
    }

    ush_log(LOG_LVL_DETAIL, "destroy hello ack and hello msg");
    ush_sync_hello_ack_destroy(&ack); // ack not needed any more.
    ush_comm_hello_msg_destroy(&hello);

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
    ush_log(LOG_LVL_INFO, "ack callback should be implemetented!!!");
    ush_assert(0);
    // ush_connect_ident   connIdentOnServer;
    // USH_PIPE_STATE      connState;
    // ush_connect_t connHdlOnClient;
    // // check ack valic
    // assert (connHdlOnClient->ident's random feild == connIdentOnServer's random
    //     && connIdentOnServer's idx feild is not 0xFFFFFFFF
    //     && state is USH_PP_STATE_NEW);
    // then ack is valid
    // and connHdlOnClient->ident = connIdentOnServer;
    return USH_RET_OK;
}
