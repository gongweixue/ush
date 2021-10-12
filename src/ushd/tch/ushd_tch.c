#include "errno.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_comm_tch.h"
#include "ush_def_pub.h"
#include "ush_log.h"

#include "ushd_tch.h"

typedef struct ushd_tch {
    mqd_t mq;
} * ushd_tch_t;


ush_ret_t
ushd_tch_create(ushd_tch_t *pTouch) {
    ush_assert(pTouch);
    *pTouch = NULL;

    ushd_tch_t tmp = (ushd_tch_t)malloc(sizeof(struct ushd_tch));
    if (!tmp) {
        ushd_log(LOG_LVL_FATAL, "ushd touch alloc failed");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_DETAIL, "alloc mem for touch, addr %p", tmp);

    tmp->mq = USH_INVALID_MQD_VALUE;
    *pTouch = tmp;

    return USH_RET_OK;
}

ush_ret_t
ushd_tch_open(ushd_tch_t touch) {
    ush_assert(touch);
    if (USH_INVALID_MQD_VALUE != touch->mq) { // maybe already opened
        ushd_log(LOG_LVL_INFO, "touch already opened");
        return USH_RET_OK;
    }

    struct mq_attr qAttr;
    memset(&qAttr, 0, sizeof(qAttr));
    qAttr.mq_maxmsg  = USH_COMM_TCH_Q_MSG_MAX_CNT;
    qAttr.mq_msgsize = USH_COMM_TCH_Q_MSG_MAX_LEN;
    touch->mq = mq_open(USH_COMM_TCH_Q_PATH,
                        O_RDONLY | O_CREAT, // read end
                        S_IRWXU  | S_IRWXG, // 0770
                        &qAttr);

    if (USH_INVALID_MQD_VALUE == touch->mq) {
        ushd_log(LOG_LVL_ERROR, "ushd touch open returns invalid value");
        return USH_RET_FAILED;
    }

    ush_log(LOG_LVL_DETAIL, "the touch queue opened, %p", touch);

    return USH_RET_OK;
}

ush_ret_t
ushd_tch_close(ushd_tch_t touch) {
    ush_assert(touch);
    if (!touch || USH_INVALID_MQD_VALUE == touch->mq) {
        ushd_log(LOG_LVL_INFO, "ushd touch already closed");
        return USH_RET_OK;
    }

    ush_log(LOG_LVL_DETAIL, "closing the touch queue %p", touch);

    if (0 != mq_close(touch->mq)) {
        ushd_log(LOG_LVL_ERROR, "ushd touch closed failed");
        return USH_RET_FAILED;
    }

    touch->mq = USH_INVALID_MQD_VALUE;

    return USH_RET_OK;
}

ush_ret_t
ushd_tch_receive(ushd_tch_t touch, ush_char_t *dest, ush_size_t sz) {
    ush_assert(sz >= USH_COMM_TCH_Q_MSG_MAX_LEN);
    if (USH_INVALID_MQD_VALUE == touch->mq || !dest) {
        ushd_log(LOG_LVL_ERROR, "ushd touch not open");
        return USH_RET_FAILED;
    }

    ushd_log(LOG_LVL_INFO, "receiving from ushd touch...");
    ush_ssize_t rcv_sz = mq_receive(touch->mq, dest, sz, NULL);

    if (-1 == rcv_sz) {
        switch (errno) {
        case EMSGSIZE:
            ush_log(LOG_LVL_ERROR, "massage too long from touch pipe");
            break;
        case EBADF:
            ush_log(LOG_LVL_ERROR, "bad mqd_t for touch");
            break;
        case EINVAL:
            ush_log(LOG_LVL_ERROR, "invalid ptr of buffer for receiving.");
        default:
            break;
        }

        ush_log(LOG_LVL_ERROR, "received ret value is -1");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_DETAIL, "ushd touch got the data");

    return USH_RET_OK;
}

ush_ret_t
ushd_tch_destroy_with_closing(ushd_tch_t *pTouch) {
    ush_assert(pTouch);
    if (!(*pTouch)) {
        ushd_log(LOG_LVL_INFO, "ushd_tch_t NULL to be destroy");
        return USH_RET_OK;
    }

    // close it anyway, no matter if it has been opened.
    ushd_tch_close(*pTouch);

    ush_log(LOG_LVL_DETAIL, "free the mem of touch %p", *pTouch);
    free(*pTouch);
    pTouch = NULL;

    return USH_RET_OK;
}
