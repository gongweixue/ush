
#include "errno.h"
#include "mqueue.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_comm_def.h"
#include "ush_def_pub.h"
#include "ush_log.h"
#include "ush_time.h"
#include "ush_tch.h"

#define USH_TCH_OPEN_RETRY_CNT    (3)
#define USH_TCH_OPEN_INTERVAL_MS  (10)

typedef struct ush_tch {
    mqd_t mq;
} * ush_tch_t;


ush_ret_t
ush_tch_send(const ush_tch_t touch, const ush_char_t *ptr, ush_size_t sz) {
    ush_assert(touch && ptr && sz > 0);
    ush_ret_t ret = USH_RET_OK;
    const ush_char_t *pMsg = (const ush_char_t *)ptr;

    int i = mq_send(touch->mq, pMsg, sz, USH_COMM_SEND_PRIO_SIG_REG);
    if (-1 == i) {
        ush_log(LOG_LVL_FATAL, "send msg failed.");
        ret = USH_RET_FAILED;
    }
    ush_log(LOG_LVL_DETAIL, "send return");

    return ret;
}

ush_ret_t
ush_tch_send_hello(const ush_tch_t                touch,
                   const ush_comm_tch_hello_t     hello,
                   const struct timespec         *pDL) {
    ush_assert(touch && hello);
    ush_ret_t ret = USH_RET_OK;
    const ush_char_t *pMsg = (const ush_char_t *)hello;

    ush_size_t sz = ush_comm_tch_hello_sizeof();
    if (pDL) { // with timeout
        int i = mq_timedsend(touch->mq, pMsg, sz, USH_COMM_SEND_PRIO_HELLO, pDL);
        if (-1 == i) {
            if ((errno == EINTR) || (errno == ETIMEDOUT)) {
                ush_log(LOG_LVL_FATAL, "send hello timeout");
                ret = USH_RET_TIMEOUT;
            } else {
                ush_log(LOG_LVL_FATAL, "send hello failed.");
                ret = USH_RET_FAILED;
            }
        }
        ush_log(LOG_LVL_DETAIL, "timedsend return");
    } else {
        ret = ush_tch_send(touch, pMsg, sz);
    }

    return ret;
}

ush_ret_t
ush_tch_close(ush_tch_t touch) {
    ush_assert(touch);
    if (USH_INVALID_MQD_VALUE == touch->mq) {
        ush_log(LOG_LVL_INFO, "touch already closed");
        return USH_RET_OK;
    }

    ush_log(LOG_LVL_DETAIL, "close touch, addr %p", touch);
    if (0 != mq_close(touch->mq)) {
        ush_log(LOG_LVL_ERROR, "touch closed failed");
        return USH_RET_FAILED;
    }

    touch->mq = USH_INVALID_MQD_VALUE;

    return USH_RET_OK;
}

ush_ret_t
ush_tch_open(ush_tch_t touch) {
    ush_assert(touch);
    if (USH_INVALID_MQD_VALUE != touch->mq) { // maybe already opened
        ush_log(LOG_LVL_INFO, "touch already open");
        return USH_RET_OK;
    }

    ush_log(LOG_LVL_DETAIL, "try to open touch, addr %p", touch);
    for (int counter = 0; counter < USH_TCH_OPEN_RETRY_CNT; ++counter) {
        touch->mq = mq_open(USH_COMM_TCH_Q_PATH, O_WRONLY);
        if (USH_INVALID_MQD_VALUE != touch->mq) { // done
            ush_log(LOG_LVL_INFO, "listener open done.");
            return USH_RET_OK;
        } else { // failed
            if (ENOENT == errno) { // file has not been create
                ush_log(LOG_LVL_ERROR, "touch not exist, retry after 500ms...");
                ush_time_delay_ms(USH_TCH_OPEN_INTERVAL_MS);
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
ush_tch_alloc(ush_tch_t *pTouch) {
    ush_assert(pTouch);
    *pTouch = NULL;

    ush_tch_t tmp = (ush_tch_t)malloc(sizeof(struct ush_tch));
    if (!tmp) {
        ush_log(LOG_LVL_FATAL, "touch alloc failed");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_DETAIL, "touch mem allocate, addr %p", tmp);

    tmp->mq = USH_INVALID_MQD_VALUE;
    *pTouch = tmp;

    return USH_RET_OK;
}

ush_ret_t
ush_tch_destroy_with_closing(ush_tch_t *pTouch) {
    ush_assert(pTouch);
    if (!(*pTouch)) {
        ush_log(LOG_LVL_INFO, "ush_tch NULL to be destroy");
        return USH_RET_OK;
    }

    // close it anyway, no matter if it has been opened.
    ush_tch_close(*pTouch);

    ush_log(LOG_LVL_DETAIL, "free mem of touch %p", *pTouch);
    free(*pTouch);
    pTouch = NULL;

    return USH_RET_OK;
}
