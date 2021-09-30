#include "ush_log.h"
#include "ush_sync.h"

#include "ush_listener_routine.h"

void
ush_listener_routine_howareyou(ush_comm_howareyou_msg_t msg) {
        ush_sync_hello_ack_t ack = ush_comm_howareyou_msg_ack_of(msg);

        ush_s32_t idx  = ush_comm_howareyou_msg_remote_idx_of(msg);
        ush_s32_t cert = ush_comm_howareyou_msg_cert_of(msg);

        if (ack) {
            ush_log(LOG_LVL_INFO, "signal the ack for hello");
            ush_sync_hello_ack_signal(ack, idx, cert);
        }
}
