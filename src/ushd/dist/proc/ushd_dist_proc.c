
#include "ush_log.h"

#include "lstnr/ush_comm_lstnr_hay.h"

#include "dist/ushd_dist_thread.h"
#include "dist/proc/ushd_dist_proc_hay.h"
#include "dist/proc/ushd_dist_proc_sig.h"

void
ushd_dist_proc(ushd_dist_thread_t thread, const ush_char_t *buf) {
    if (!thread || !buf) {
        ushd_log(LOG_LVL_ERROR, "null ptr of parameters");
        return;
    }

    switch (((dist_fifo_msg_d*)buf)->type) {
    case USHD_DIST_FIFO_MSG_TYPE_HAY:
        ushd_log(LOG_LVL_INFO, "sending hay-msg %p", buf);
        ushd_dist_proc_hay_send(thread, (const dist_fifo_msg_hay *)buf);
        break;

    case USHD_DIST_FIFO_MSG_TYPE_SIG_REG_ACK:
        ushd_log(LOG_LVL_INFO, "sending sig-reg-msg %p", buf);
        ushd_dist_proc_sig_send_reg_ack(
            thread, (const dist_fifo_msg_sig_reg_ack *)buf);
        break;

    default:
        ushd_log(LOG_LVL_ERROR, "wrong dist fifo msg type");
        break;
    }

    return;
}
