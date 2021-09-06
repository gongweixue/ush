#include "mqueue.h"
#include "fcntl.h"
#include "ush_impl_touch.h"
#include "ush_impl_protocol.h"
#include "ush_impl_log.h"

#include <string.h>
#include <stdio.h>

int main () {
    struct mq_attr qAttr;
    memset(&qAttr, 0, sizeof(qAttr));
    qAttr.mq_maxmsg  = USH_IMPL_TOUCH_Q_MSG_MAX_CNT;
    qAttr.mq_msgsize = USH_IMPL_TOUCH_Q_MSG_MAX_LEN;
    mqd_t mq = mq_open(USH_IMPL_TOUCH_Q_PATH,
                       O_RDONLY | O_CREAT, // read end
                       S_IRWXU | S_IRWXG, // 0770
                       &qAttr);

    if (-1 == mq) {
        ush_log(0, "open failed\n");
        return USH_RET_FAILED;
    }

    while (1) {
        char  buff[USH_IMPL_TOUCH_Q_MSG_MAX_LEN];
        ush_log(0, "receiving touch\n");
        ush_ssize_t rcv_sz = mq_receive(mq, buff, sizeof(buff), NULL);

        if (-1 == rcv_sz) {
            ush_log(0, "ERROR rcv_sz\n");
            continue;
        }

        printf("%ld\n", rcv_sz);
        ush_impl_touch_msg_t *pTouch = (ush_impl_touch_msg_t *)buff;
        printf("%d %s\n", pTouch->id, pTouch->name);

    }

    if (mq_close(mq) == -1) {
        ush_log(0, "close touch queue failed\n");
        return USH_RET_FAILED;
    }
    if (mq_unlink(USH_IMPL_TOUCH_Q_PATH) == -1) {
        ush_log(0, "unlink touch queue failed\n");
        return USH_RET_FAILED;
    }

    return 0;
}
