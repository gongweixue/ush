#include "assert.h"
#include "errno.h"
#include "mqueue.h"
#include "fcntl.h"
#include "string.h"

#include "ush_comm_touch.h"
#include "ush_comm_protocol.h"
#include "ush_log.h"
#include "ush_pipe.h"
#include "ush_pipe_cr.h"
#include "ush_pipe_pub.h"

typedef struct touch_handle_t {
    mqd_t mq;
} touch_handle_t;

static ush_ret_t hello_there(const ush_char_t *pName, const timespec *pDL);
static ush_ret_t open_touch(touch_handle_t *pHdl);
static ush_ret_t close_touch(const touch_handle_t *pHdl);
static ush_ret_t fill_msg_hello(touch_msg_hello_t *pHello, const char* pName);
static ush_ret_t send_hello(const touch_handle_t *pHdl, const touch_msg_hello_t *pHello, const timespec *pDL);

static ush_ret_t init_ack_hdl(const ush_pipe_ack_sync_handle_t *pHdl);
static ush_ret_t wait_ack_sync(const ush_pipe_ack_sync_handle_t *pHdl);
static ush_ret_t destory_ack_hdl(const ush_pipe_ack_sync_handle_t *pHdl);

ush_ret_t ush_pipe_create(
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
    assert(strlen(pName) < USH_COMM_TOUCH_Q_HELLO_NAME_LEN);

    // for timeout
    timespec deadline;
    timespec *pDL = NULL;
    if (0 != timeout) {
        pDL = &deadline;
        clock_gettime(CLOCK_MONOTONIC, pDL); // since boot
        pDL->tv_sec += timeout + 1; // extra 1s for poor perf.
    }
    ush_ret_t ret = ush_pipe_cr_open(pName);
    if (USH_RET_OK != ret) {
        return ret;
    }

    ush_pipe_ack_sync_handle_t ack_hdl;
    ret = init_ack_hdl(&ack_hdl);
    if (USH_RET_OK != ret) {
        destory_ack_hdl(&ack_hdl);
        ush_pipe_cr_close();
        return ret;
    }

    ret = hello_there(pName, pDL);
    if (USH_RET_OK != ret) {
        destory_ack_hdl(&ack_hdl);
        ush_pipe_cr_close();
        return ret;
    }

    ret = wait_ack_sync(&ack_hdl);
    destory_ack_hdl(&ack_hdl);
    if (USH_RET_OK != ret) {
        return ret;
    }

    return USH_RET_OK;
}

static ush_ret_t hello_there(const char *pName, const timespec *pDL) {

    // param valid
    if (!pName || strlen(pName) >= USH_COMM_TOUCH_Q_HELLO_NAME_LEN) {
        return USH_RET_WRONG_PARAM;
    }

    // open touch
    touch_handle_t hdl;
    if (USH_RET_OK != open_touch(&hdl)) {
        ush_log(USH_LOG_LVL_ERR, "touch open failed\n");
        return USH_RET_FAILED;
    }

    // prepare hello msg
    touch_msg_hello_t hello_msg;
    fill_msg_hello(&hello_msg, pName);

    // send with or without timeout
    ush_ret_t ret = send_hello(&hdl, &hello_msg, pDL);
    if (USH_RET_OK != ret) {
        ush_log(USH_LOG_LVL_ERR, "hello failed\n");
    }

    close_touch(&hdl);

    return ret;
}

static ush_ret_t fill_msg_hello(touch_msg_hello_t *pHello, const char* pName) {
    pHello->desc.catalog = USH_COMM_TOUCH_MSG_CATALOG_HELLO;
    strcpy(pHello->name, pName);
    pHello->nameSz = strlen(pName) + 1; // with \0

    return USH_RET_OK;
}

static ush_ret_t open_touch(touch_handle_t *pHdl) {
    pHdl->mq = mq_open(USH_COMM_TOUCH_Q_PATH, O_WRONLY);
    if (-1 == pHdl->mq) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}

static ush_ret_t close_touch(const touch_handle_t *pHdl) {
    return (-1 == mq_close(pHdl->mq)) ? USH_RET_FAILED : USH_RET_OK;
}

static ush_ret_t send_hello(const touch_handle_t *pHdl, const touch_msg_hello_t *pHello, const timespec *pDL) {
    ush_ret_t ret = USH_RET_OK;
    const ush_char_t *pMsg = (const ush_char_t *)pHello;

    if (pDL) { // with timeout
        if (-1 == mq_timedsend(pHdl->mq, pMsg, sizeof(touch_msg_hello_t), 0, pDL)) {
            if ((errno == EINTR) || (errno == ETIMEDOUT)) {
                ush_log(USH_LOG_LVL_ERR, "send hello timeout\n");
                ret = USH_RET_TIMEOUT;
            } else {
                ush_log(USH_LOG_LVL_ERR, "send hello failed.\n");
                ret = USH_RET_FAILED;
            }
        }
    } else {
        if (-1 == mq_send(pHdl->mq, pMsg, sizeof(touch_msg_hello_t), 0)) {
            ush_log(USH_LOG_LVL_ERR, "send hello failed.\n");
            ret = USH_RET_FAILED;
        }
    }

    return USH_RET_OK;
}

static ush_ret_t init_ack_hdl(const ush_pipe_ack_sync_handle_t *pHdl) {
    return USH_RET_OK;
}
static ush_ret_t wait_ack_sync(const ush_pipe_ack_sync_handle_t *pHdl) {
    return USH_RET_OK;
}
static ush_ret_t destory_ack_hdl(const ush_pipe_ack_sync_handle_t *pHdl) {
    return USH_RET_OK;
}
