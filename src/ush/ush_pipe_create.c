
#include "errno.h"
#include "fcntl.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#include "ush_assert.h"
#include "ush_comm_touch.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_hello.h"
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
    ush_pp_mode_t     mode,
    ush_u32_t         flag,
    ush_u16_t         timeout,
    ush_vptr_t       *pParams,
    ush_size_t        paramCnt,
    ush_pp_hdl_t     *pHdl)
{
    // params valid
    if (!pName || !pHdl || USH_PP_MODE_MAX_GUARD <= mode) {
        ush_log(LOG_LVL_ERROR, "wrong params for pipe create.");
        return USH_RET_WRONG_PARAM;
    }
    ush_assert(strlen(pName) < USH_HELLO_NAME_LEN_MAX);
    ush_assert(strlen(pName) >= USH_HELLO_NAME_LEN_MIN);

    ush_ret_t ret = USH_RET_OK;
    // for timeout
    timespec deadline;
    timespec *pDL = NULL;
    if (0 != timeout) {
        pDL = &deadline;
        ret = realize_timeout(pDL, timeout);
        if(USH_RET_OK != ret) {
            ush_log(LOG_LVL_ERROR, "realize timeout failed");
            goto RET;
        }
    }

    ush_connect_t conn = NULL;
    ret = ush_connect_create(&conn);
    if (USH_RET_OK != ret) {
        goto RET;
    }

    ret = send_hello_and_wait(pName, pDL, conn);

    if (USH_RET_OK == ret) {
        *pHdl = (ush_s64_t)conn;
    } else {
        *pHdl = -1;
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
        return USH_RET_WRONG_PARAM;
    }

    // use ack to wait feedback
    ush_sync_hello_ack_t ack = NULL;
    ush_ret_t ret = ush_sync_hello_ack_create(&ack, conn);
    if (USH_RET_OK != ret) {
        return ret;
    }

    // prepare hello msg
    ush_hello_msg_t hello;
    ush_hello_create(&hello, pName, ack, ush_connect_generate_cert(pName));

    // send with or without timeout
    ush_touch_t touch = NULL;
    ush_connect_get_touch(conn, &touch);
    ret = ush_touch_send_hello(touch, hello, pDL);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "hello failed");
    } else {
        ret = ush_sync_hello_ack_wait(ack, pDL, get_info_from_hello_ack_cb);
    }


    ush_sync_hello_ack_destroy(&ack); // ack not needed any more.
    ush_hello_destroy(&hello);
    if (USH_RET_OK != ret) {
        return ret;
    }

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

    ptr->tv_sec += timeout + 1;

    return USH_RET_OK;
}

static ush_ret_t
get_info_from_hello_ack_cb(ush_sync_hello_ack_t ack) {
    ush_assert(ack);
    ush_log(LOG_LVL_INFO, "ack callback should be implemetented!!!");
    // ush_connect_ident   connIdentOnServer;
    // ush_pp_state_t      connState;
    // ush_connect_t connHdlOnClient;
    // // check ack valic
    // assert (connHdlOnClient->ident's random feild == connIdentOnServer's random
    //     && connIdentOnServer's idx feild is not 0xFFFFFFFF
    //     && state is USH_PP_STATE_NEW);
    // then ack is valid
    // and connHdlOnClient->ident = connIdentOnServer;
    return USH_RET_OK;
}
