#include "assert.h"
#include "errno.h"
#include "mqueue.h"
#include "fcntl.h"
#include "string.h"
#include "time.h"

#include "ush_pipe.h"
#include "ush_impl_touch.h"
#include "ush_impl_protocol.h"
#include "ush_impl_log.h"

#include <stdio.h>

static ush_ret_t impl_create_hello(
    const ush_char_t *pName,
    ush_pp_mode_t     mode,
    ush_u32_t         flag,
    const struct timespec   *pDeadline)
{
    if (!pName || USH_PP_MODE_MAX_GUARD <= mode) {
        return USH_RET_WRONG_PARAM;
    }
    // msg queue desc
    mqd_t mq = mq_open(USH_IMPL_TOUCH_Q_PATH, O_WRONLY);
    if (-1 == mq) {
        ush_log(0, "open failed\n");
        return USH_RET_FAILED;
    }

    ush_impl_touch_msg_t touch;
    touch.id = USH_IMPL_TOUCH_Q_MSG_ID_HELLO;
    strcpy(touch.name, pName);

    // send with or without timeout
    const ush_char_t *pBuf = (const ush_char_t *)&touch;
    if (pDeadline) {
        ush_s32_t res = mq_timedsend(mq, pBuf, sizeof(touch),
                                     USH_IMPL_TOUCH_Q_MSG_ID_HELLO_PROI,
                                     pDeadline);
        if (-1 == res) {
            if ((errno == EINTR) || (errno == ETIMEDOUT)) {
                ush_log(0, "mq_timedsend timeout\n");
                mq_close(mq);
                return USH_RET_TIMEOUT;
            } else {
                ush_log(0, "mq_timedsend failed.\n");
                mq_close(mq);
                return USH_RET_FAILED;
            }
        }
    } else {
        ush_s32_t res = mq_send(mq, pBuf, sizeof(touch),
                                USH_IMPL_TOUCH_Q_MSG_ID_HELLO_PROI);
        if (-1 == res) {
            ush_log(0, "mq_send failed.\n");
            mq_close(mq);
            return USH_RET_FAILED;
        }
    }

    // only close the desc, do @@NOT unlink it
    if (mq_close(mq) == -1) {
        ush_log(0, "client close touch queue failed\n");
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}

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
        ush_log(0, "wrong params for pipe create.\n");
        return USH_RET_WRONG_PARAM;
    }
    assert(strlen(pName) <= USH_IMPL_TOUCH_Q_PIPENAME_LEN);

    // in case of timeout
    struct timespec deadline;
    struct timespec *pDeadline = NULL;
    if (0 != timeout) {
        pDeadline = &deadline;
        clock_gettime(CLOCK_MONOTONIC, pDeadline); // since boot
        pDeadline->tv_sec += timeout + 1; // extra 1s for poor perf.
    }

    ush_ret_t hello_ret = impl_create_hello(pName, mode, flag, pDeadline);
    if (USH_RET_OK != hello_ret) {
        return hello_ret;
    }


    //////

    return USH_RET_OK;
}
