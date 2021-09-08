#include "assert.h"
#include "errno.h"
#include "fcntl.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"

#include "ush_comm_touch.h"
#include "ush_comm_protocol.h"
#include "ush_log.h"
#include "ush_pipe.h"
// #include "ush_pipe_cr.h"
#include "ush_pipe_pub.h"

typedef struct touch_handle_t {
    mqd_t mq;
} touch_handle_t;

static ush_ret_t hello_there(const ush_char_t *pName, const timespec *pDL);
static ush_ret_t open_touch(touch_handle_t *pHdl);
static ush_ret_t close_touch(const touch_handle_t *pHdl);
static ush_ret_t fill_msg_hello(touch_msg_hello_t *pHello, const char* pName);
static ush_ret_t send_hello(const touch_handle_t *pHdl, const touch_msg_hello_t *pHello, const timespec *pDL);

static ush_ret_t create_ack_hdl(ush_pipe_ack_sync_handle_t *pHdl);
static ush_ret_t wait_ack_sync(ush_pipe_ack_sync_handle_t *pHdl, const timespec *pDL);
static ush_ret_t destroy_ack_hdl(ush_pipe_ack_sync_handle_t *pHdl);

static ush_ret_t realize_timeout(timespec *ptr, ush_u16_t timeout);

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
        if(USH_RET_OK != realize_timeout(pDL, timeout)) {
            return USH_RET_FAILED;
        }
    }
    // ush_ret_t ret = ush_pipe_cr_open(pName);
    // if (USH_RET_OK != ret) {
    //     return ret;
    // }


    ush_pipe_ack_sync_handle_t *pAckHdl = NULL;
    ush_ret_t ret = create_ack_hdl(pAckHdl);
    if (USH_RET_OK != ret) {
        // ush_pipe_cr_close();
        destroy_ack_hdl(pAckHdl);
        return ret;
    }

    ret = hello_there(pName, pDL);
    if (USH_RET_OK != ret) {
        // ush_pipe_cr_close();
        destroy_ack_hdl(pAckHdl);
        return ret;
    }

    ret = wait_ack_sync(pAckHdl, pDL);
    destroy_ack_hdl(pAckHdl); // not needed any more.
    if (USH_RET_OK != ret) {
        // ush_pipe_cr_close();
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
    assert(pHello && pName);
    pHello->desc.catalog = USH_COMM_TOUCH_MSG_CATALOG_HELLO;
    strcpy(pHello->name, pName);
    pHello->nameSz = strlen(pName) + 1; // with \0

    return USH_RET_OK;
}

static ush_ret_t open_touch(touch_handle_t *pHdl) {
    assert(pHdl);
    pHdl->mq = mq_open(USH_COMM_TOUCH_Q_PATH, O_WRONLY);
    if (-1 == pHdl->mq) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}

static ush_ret_t close_touch(const touch_handle_t *pHdl) {
    assert(pHdl);
    if (mq_close(pHdl->mq)) {
        ush_log(USH_LOG_LVL_ERROR, "close touch queue failed\n");
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}

static ush_ret_t send_hello(const touch_handle_t *pHdl, const touch_msg_hello_t *pHello, const timespec *pDL) {
    assert(pHdl && pHello);
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

static ush_ret_t create_ack_hdl(ush_pipe_ack_sync_handle_t *pHdl) {
    void *pMem = malloc(sizeof(ush_pipe_ack_sync_handle_t));
    if (!pMem) {
        return USH_RET_OUT_OF_MEM;
    }

    ush_pipe_ack_sync_handle_t *ptr = (ush_pipe_ack_sync_handle_t *)pMem;

    if (0 != pthread_mutex_init(&ptr->mutex, NULL)) {
        free(pMem);
        ush_log(ERR, "hello ack sync handle create failed\n");
        return USH_RET_FAILED;
    }

    // TODO: need to judge the return value???
    pthread_condattr_init(&ptr->condattr);
    pthread_condattr_setclock(&ptr->condattr, CLOCK_MONOTONIC);

    if (0 != pthread_cond_init(&ptr->cond, &ptr->condattr)) {
        pthread_mutex_destroy(&ptr->mutex);
        pthread_condattr_destroy(&ptr->condattr);
        free(pMem);
        ush_log(ERR, "hello ack sync handle create failed\n");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}
static ush_ret_t wait_ack_sync(ush_pipe_ack_sync_handle_t *pHdl, const timespec *pDL) {
    assert(pHdl);

    pthread_mutex_lock(&pHdl->mutex);

    int ret = USH_RET_OK;
    switch (pthread_cond_timedwait(&pHdl->cond, &pHdl->mutex, pDL)) {
    case 0:
        break;
    case ETIMEDOUT:
        ush_log(ERR, "cond wait timeout\n");
        ret = USH_RET_TIMEOUT;
        break;
    default :
        ush_log(ERR, "cond wait failed\n");
        ret = USH_RET_FAILED;
        break;
    }

    // may be something should be done here.

    pthread_mutex_unlock(&pHdl->mutex);
    return ret;
}
static ush_ret_t destroy_ack_hdl(ush_pipe_ack_sync_handle_t *pHdl) {
    pthread_mutex_destroy(&pHdl->mutex);
    pthread_condattr_destroy(&pHdl->condattr);
    pthread_cond_destroy(&pHdl->cond);
    return USH_RET_OK;
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
