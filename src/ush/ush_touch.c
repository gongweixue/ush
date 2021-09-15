
#include "errno.h"
#include "mqueue.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_log.h"
#include "ush_time.h"
#include "ush_touch.h"

#define USH_TOUCH_OPEN_RETRY_CNT    3
#define USH_TOUCH_OPEN_INTERVAL_MS  500

typedef struct ush_touch {
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
                ush_log(LOG_LVL_FATAL, "send hello timeout");
                ret = USH_RET_TIMEOUT;
            } else {
                ush_log(LOG_LVL_FATAL, "send hello failed.");
                ret = USH_RET_FAILED;
            }
        }
    } else {
        if (-1 == mq_send(touch->mq, pMsg, ush_hello_msg_size(), 0)) {
            ush_log(LOG_LVL_FATAL, "send hello failed.");
            ret = USH_RET_FAILED;
        }
    }

    return ret;
}

ush_ret_t
ush_touch_close(ush_touch_t touch) {
    ush_assert(touch);
    if (-1 == touch->mq) {
        ush_log(LOG_LVL_INFO, "touch already closed");
        return USH_RET_OK;
    }

    if (0 != mq_close(touch->mq)) {
        ush_log(LOG_LVL_ERROR, "touch closed failed");
        return USH_RET_FAILED;
    }

    touch->mq = -1;

    return USH_RET_OK;
}

ush_ret_t
ush_touch_open(ush_touch_t touch) {
    ush_assert(touch);
    if (-1 != touch->mq) { // maybe already opened
        ush_log(LOG_LVL_INFO, "touch already open");
        return USH_RET_OK;
    }

    for (int counter = 0; counter < USH_TOUCH_OPEN_RETRY_CNT; ++counter) {
        touch->mq = mq_open(USH_COMM_TOUCH_Q_PATH, O_WRONLY);
        if (-1 != touch->mq) { // done
            ush_log(LOG_LVL_INFO, "listener open done.");
            return USH_RET_OK;
        } else { // failed
            if (ENOENT == errno) { // file has not been create
                ush_log(LOG_LVL_ERROR, "touch not exist, retry after 500ms...");
                ush_time_delay_ms(USH_TOUCH_OPEN_INTERVAL_MS);
                continue;
            } else {
                ush_log(LOG_LVL_FATAL, "touch open failed");
                break;
            }
        }
    }

    return USH_RET_FAILED;
}

ush_ret_t
ush_touch_alloc(ush_touch_t *pTouch) {
    ush_assert(pTouch);
    *pTouch = NULL;

    ush_touch_t tmp = (ush_touch_t)malloc(sizeof(struct ush_touch));
    if (!tmp) {
        ush_log(LOG_LVL_FATAL, "touch alloc failed");
        return USH_RET_OUT_OF_MEM;
    }

    tmp->mq = -1;
    *pTouch = tmp;

    return USH_RET_OK;
}

ush_ret_t
ush_touch_destroy_with_closing(ush_touch_t *pTouch) {
    ush_assert(pTouch);
    if (!(*pTouch)) {
        ush_log(LOG_LVL_INFO, "ush_touch NULL to be destroy");
        return USH_RET_OK;
    }

    // close it anyway, no matter if it has been opened.
    ush_touch_close(*pTouch);

    free(*pTouch);
    pTouch = NULL;

    return USH_RET_OK;
}
