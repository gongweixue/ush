
#include "ush_assert.h"
#include "ush_log.h"

#include "ush_comm_howareyou_msg.h"

#include "ushd_dist_proc.h"

void dist_proc_send_howareyou(mqd_t mq, dist_fifo_msg_howareyou *msg) {
    ush_assert(-1 != mq && msg);

    // construct msg over mqueue
    ush_comm_howareyou_msg_t howareyou =
        ush_comm_howareyou_msg_create(msg->ack_sync, msg->idx, msg->cert);
    if (!howareyou) {
        ushd_log(LOG_LVL_ERROR, "howareyou msg mem alloc failed.");
        return;
    }
    ush_comm_howareyou_msg_testpoint(howareyou);

    ushd_log(LOG_LVL_DETAIL, "sending msg from %p ......", howareyou);
    int tmp = mq_send(mq, (char*)howareyou,
                      ush_comm_howareyou_msg_sizeof(),
                      USH_COMM_HOWAREYOU_MSG_PRIO);
    if (-1 == tmp) {
        ushd_log(LOG_LVL_FATAL, "send howareyou %p failed.", howareyou);
    }
    ush_comm_howareyou_msg_destroy(&howareyou);
}
