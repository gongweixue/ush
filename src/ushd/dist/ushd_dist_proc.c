
#include "ush_log.h"

#include "ush_comm_lstnr_hay.h"

#include "dist/ushd_dist_thread.h"
#include "dist/ushd_dist_proc.h"
#include "dist/ushd_dist_proc_hay.h"
#include "dist/ushd_dist_proc_sig.h"
#include "dist/ushd_dist_proc_cmd.h"

void
ushd_dist_proc(ushd_dist_thread_t thread, ush_char_t *buf) {
    if (!thread || !buf) {
        ushd_log(LOG_LVL_ERROR, "null ptr of parameters");
        return;
    }

    switch (((dist_fifo_msg_d*)buf)->type) {
    case USHD_DIST_FIFO_MSG_TYPE_HAY:
        ushd_log(LOG_LVL_INFO, "sending hay-msg %p", buf);
        ushd_dist_proc_hay_send(thread, (const dist_fifo_msg_hay *)buf);
        break;

    case USHD_DIST_FIFO_MSG_TYPE_SIGREG_ACK:
        ushd_log(LOG_LVL_INFO, "sending sig-reg-msg %p", buf);
        ushd_dist_proc_sig_send_reg_ack(
            thread, (const dist_fifo_msg_sigreg_ack *)buf);
        break;

    case USHD_DIST_FIFO_MSG_TYPE_SIG_UPD:
        ushd_log(LOG_LVL_INFO, "sending sig_upd msg %p", buf);
        ushd_dist_proc_sig_send_update(
            thread, (const dist_fifo_msg_sig_upd *)buf);
        break;

    case USHD_DIST_FIFO_MSG_TYPE_STOP:
        ushd_log(LOG_LVL_INFO, "request to stop the dist thread");
        ushd_dist_proc_cmd_stop_dist(thread);
        break;

    default:
        ushd_log(LOG_LVL_ERROR, "wrong dist fifo msg type");
        break;
    }

    return;
}
