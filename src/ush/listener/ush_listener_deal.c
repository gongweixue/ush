
#include "ush_assert.h"
#include "ush_log.h"

#include "ush_comm_howareyou_msg.h"
#include "ush_listener_deal.h"
#include "ush_sync.h"

ush_ret_t
ush_listener_deal(ush_listener_msg_description *desc) {
    ush_assert(desc);
    if (USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU == desc->catalog) {
        ush_comm_howareyou_msg_t msg = (ush_comm_howareyou_msg_t)desc;
        ush_comm_howareyou_msg_testpoint(msg);

        // execute no matter ok or not ok,
        ush_log(LOG_LVL_INFO, "signal the ack for hello");
        ush_sync_hello_ack_t ack =
            (ush_sync_hello_ack_t)ush_comm_howareyou_msg_ack(msg);
        ush_s32_t idx  = ush_comm_howareyou_msg_remote_idx(msg);
        ush_s32_t cert = ush_comm_howareyou_msg_cert(msg);

            if (ack) {
                ush_sync_hello_ack_signal(ack, idx, cert);
            }
    }

    return USH_RET_OK;
}
