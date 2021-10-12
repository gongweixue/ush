#include "ush_log.h"
#include "ush_sync.h"

#include "ush_lstnr_proc_hay.h"

void
ush_lstnr_proc_hay(const ush_comm_lstnr_hay_t msg) {
        ush_sync_hello_ack_t ack = ush_comm_lstnr_hay_ack_of(msg);

        ush_s32_t idx  = ush_comm_lstnr_hay_remote_idx_of(msg);
        ush_s32_t cert = ush_comm_lstnr_hay_cert_of(msg);

        if (ack) {
            ush_log(LOG_LVL_INFO, "signal the ack for hello");
            ush_sync_hello_ack_signal(ack, idx, cert);
        }
}
