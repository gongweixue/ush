#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_comm_touch.h"
#include "ush_log.h"

#include "ushd_touch.h"

typedef struct ushd_touch {
    mqd_t mq;
} * ushd_touch_t;


ush_ret_t
ushd_touch_alloc(ushd_touch_t *pTouch) {
    ush_assert(pTouch);
    *pTouch = NULL;

    ushd_touch_t tmp = (ushd_touch_t)malloc(sizeof(struct ushd_touch));
    if (!tmp) {
        ushd_log(LOG_LVL_ERROR, "ushd touch alloc failed\n");
        return USH_RET_OUT_OF_MEM;
    }

    tmp->mq = -1;
    *pTouch = tmp;

    return USH_RET_OK;
}

ush_ret_t
ushd_touch_open(ushd_touch_t touch) {
    ush_assert(touch);
    if (-1 != touch->mq) { // maybe already opened
    ushd_log(LOG_LVL_INFO, "touch already opened");
        return USH_RET_OK;
    }

    struct mq_attr qAttr;
    memset(&qAttr, 0, sizeof(qAttr));
    qAttr.mq_maxmsg  = USH_COMM_TOUCH_Q_MSG_MAX_CNT;
    qAttr.mq_msgsize = USH_COMM_TOUCH_Q_MSG_MAX_LEN;
    touch->mq = mq_open(USH_COMM_TOUCH_Q_PATH,
                        O_RDONLY | O_CREAT, // read end
                        S_IRWXU  | S_IRWXG, // 0770
                        &qAttr);

    if (-1 == touch->mq) {
        ushd_log(LOG_LVL_ERROR, "ushd touch open returns -1\n");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}

ush_ret_t
ushd_touch_close(ushd_touch_t touch) {
    ush_assert(touch);
    if (!touch || -1 == touch->mq) {
        return USH_RET_OK;
    }

    if (0 != mq_close(touch->mq)) {
        return USH_RET_FAILED;
    }

    touch->mq = -1;

    return USH_RET_OK;
}

ush_ret_t
ushd_touch_destroy_with_closing(ushd_touch_t *pTouch) {
    ush_assert(pTouch);
    if (!(*pTouch)) {
        ushd_log(LOG_LVL_INFO, "ushd_touch_t NULL to be destroy\n");
        return USH_RET_OK;
    }

    // close it anyway, no matter if it has been opened.
    ushd_touch_close(*pTouch);

    free(*pTouch);
    pTouch = NULL;

    return USH_RET_OK;
}
