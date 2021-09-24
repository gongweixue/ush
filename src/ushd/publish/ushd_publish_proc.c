
#include "ush_assert.h"
#include "ush_log.h"

#include "ush_comm_howareyou_msg.h"

#include "ushd_publish_proc.h"

void publish_proc_send_howareyou(mqd_t mq, publish_fifo_msg_howareyou *msg) {
    ush_assert(-1 != mq && msg);
    ush_vptr_t ack  = msg->ack_sync;
    ush_u64_t ident = msg->ident;

    // construct msg over mqueue
    ush_comm_howareyou_msg buf = {
        {USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU},
        ack,
        ident
    };

    ushd_log(LOG_LVL_DETAIL, "sending msg %p", &buf);
    int tmp = mq_send(mq, (char*)&buf, sizeof(buf), USH_COMM_HOWAREYOU_MSG_PRIO);
    if (-1 == tmp) {
        ushd_log(LOG_LVL_FATAL, "send howareyou %p failed.", msg);
    }
}
