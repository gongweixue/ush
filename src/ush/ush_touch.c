
#include "errno.h"
#include "mqueue.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_log.h"
#include "ush_touch.h"

typedef struct touch_t {
    mqd_t mq;
} * ush_touch_t;

ush_ret_t
ush_touch_send_hello(const ush_touch_t     touch,
                     const ush_hello_msg_t hello,
                     const struct timespec *pDL) {
    ush_assert(touch && hello);
    ush_ret_t ret = USH_RET_OK;
    const ush_char_t *pMsg = (const ush_char_t *)hello;

    if (pDL) { // with timeout
        if (-1 == mq_timedsend(touch->mq, pMsg, ush_hello_msg_size(), 0, pDL)) {
            if ((errno == EINTR) || (errno == ETIMEDOUT)) {
                ush_log(USH_LOG_LVL_ERROR, "send hello timeout\n");
                ret = USH_RET_TIMEOUT;
            } else {
                ush_log(USH_LOG_LVL_ERROR, "send hello failed.\n");
                ret = USH_RET_FAILED;
            }
        }
    } else {
        if (-1 == mq_send(touch->mq, pMsg, ush_hello_msg_size(), 0)) {
            ush_log(USH_LOG_LVL_ERROR, "send hello failed.\n");
            ret = USH_RET_FAILED;
        }
    }

    return USH_RET_OK;
}

ush_ret_t ush_touch_close(ush_touch_t touch) {
    assert(touch);
    if (-1 == touch->mq) {
        return USH_RET_OK;
    }

    if (0 != mq_close(touch->mq)) {
        return USH_RET_FAILED;
    }

    touch->mq = -1;

    return USH_RET_OK;
}

ush_ret_t ush_touch_open(ush_touch_t touch) {
    ush_assert(touch);
    if (-1 != touch->mq) { // maybe already opened
        return USH_RET_OK;
    }

    touch->mq = mq_open(USH_COMM_TOUCH_Q_PATH, O_WRONLY);
    if (-1 == touch->mq) {
        ush_log(USH_LOG_LVL_ERROR, "touch open returns failed\n");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}

ush_ret_t ush_touch_alloc(ush_touch_t *pTouch) {
    ush_touch_t tmp = (ush_touch_t)malloc(sizeof(struct touch_t));
    if (!tmp) {
        ush_log(USH_LOG_LVL_ERROR, "touch alloc failed\n");
        return USH_RET_OUT_OF_MEM;
    }

    tmp->mq = -1;
    *pTouch = tmp;

    return USH_RET_OK;
}

ush_ret_t ush_touch_destroy_with_closing(ush_touch_t *pTouch) {
    ush_assert(pTouch);
    if (!(*pTouch)) {
        ush_log(USH_LOG_LVL_INFO, "touch_t NULL to be destroy\n");
        return USH_RET_OK;
    }

    // close it anyway, no matter if it has been opened.
    ush_touch_close(*pTouch);

    free(*pTouch);
    pTouch = NULL;

    return USH_RET_OK;
}
