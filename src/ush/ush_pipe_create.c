#include "assert.h"
#include "errno.h"
#include "fcntl.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#include "ush_comm_touch.h"
#include "ush_comm_protocol.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_hello.h"
#include "ush_pipe_pub.h"
#include "ush_pipe_sync.h"
#include "ush_pipe_touch.h"



typedef struct timespec timespec;

static ush_ret_t hello_and_wait(const ush_char_t *pName,
                                const struct timespec *pDL,
                                connect_t *pConn);

static ush_ret_t realize_timeout(timespec *ptr, ush_u16_t timeout);

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
        ush_log(USH_LOG_LVL_ERR, "wrong params for pipe create.\n");
        return USH_RET_WRONG_PARAM;
    }
    assert(strlen(pName) < USH_COMM_TOUCH_Q_HELLO_NAME_LEN_MAX);
    assert(strlen(pName) >= USH_COMM_TOUCH_Q_HELLO_NAME_LEN_MIN);

    ush_ret_t ret = USH_RET_OK;
    // for timeout
    timespec deadline;
    timespec *pDL = NULL;
    if (0 != timeout) {
        pDL = &deadline;
        ret = realize_timeout(pDL, timeout);
        if(USH_RET_OK != ret) {
            goto RET;
        }
    }

    connect_t *pConn = NULL;
    ret = connect_alloc(pConn);
    if (USH_RET_OK != ret) {
        goto RET;
    }
    ret = connect_init(pConn);
    if (USH_RET_OK != ret) {
        ush_log(ERR, "connection init failed\n");
        goto DESTROY_CONN;
    }

    ret = hello_and_wait(pName, pDL, pConn);

    if (USH_RET_OK == ret) {
        *pHdl = (ush_s64_t)pConn;
    } else {
        *pHdl = -1;
    }

DESTROY_CONN:
    connect_destroy(pConn);

RET:
    return ret;
}

static ush_ret_t hello_and_wait(const char *pName, const timespec *pDL, connect_t *pConn) {
    // param valid
    if (pName || pDL || pConn) {
        return USH_RET_WRONG_PARAM;
    }

    // use ack to wait feedback
    sync_hello_ack_t *pAck = NULL;
    ush_ret_t ret = ush_sync_hello_ack_create(pAck);
    if (USH_RET_OK != ret) {
        return ret;
    }

    // prepare hello msg
    ush_pipe_msg_hello_t hello_msg;
    ush_pipe_hello_load(&hello_msg, pName, pAck);

    // send with or without timeout
    ush_pipe_touch_t * pTouch = NULL;
    connect_get_touch(pConn, pTouch);
    ret = ush_pipe_touch_send_hello(pTouch, &hello_msg, pDL);
    if (USH_RET_OK != ret) {
        ush_log(USH_LOG_LVL_ERR, "hello failed\n");
    } else {
        ret = ush_sync_hello_ack_wait(pAck, pDL);
    }


    ush_sync_hello_ack_destroy(pAck); // not needed any more.
    if (USH_RET_OK != ret) {
        return ret;
    }

    return ret;
}


static ush_ret_t realize_timeout(timespec *ptr, ush_u16_t timeout) {
    assert(ptr);
    if (-1 == clock_gettime(CLOCK_MONOTONIC, ptr)) {
        ush_log(USH_LOG_LVL_ERROR, "clock_gettime failed\n");
        return USH_RET_FAILED;
    } else {
        ptr->tv_sec += timeout + 1;
    }

    return USH_RET_OK;
}
