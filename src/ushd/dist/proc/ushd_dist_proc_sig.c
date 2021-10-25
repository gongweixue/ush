#include "ush_type_pub.h"
#include "lstnr/ush_comm_lstnr_sig_upd.h"
#include "lstnr/ush_comm_lstnr_sigreg_ack.h"
#include "ushd_dist_proc_sig.h"

ush_ret_t
ushd_dist_proc_sig_send_reg_ack(ushd_dist_thread_t thread,
                                const dist_fifo_msg_sig_reg_ack *msg) {
    if (!thread || !msg) {
        ushd_log(LOG_LVL_ERROR, "null ptr of param");
        return USH_RET_WRONG_PARAM;
    }

    ush_bool_t   success = msg->success;
    ush_sig_id_t sigid   = msg->sigid;
    ush_pipe_t   pipe    = msg->pipe;
    ush_pvoid_t  done    = msg->done;

    ush_comm_lstnr_sigreg_ack_t ack =
        ush_comm_lstnr_sigreg_ack_create(success, sigid, pipe, done);
    if (!ack) {
        ushd_log(LOG_LVL_ERROR, "ack msg mem alloc failed.");
        return USH_RET_OUT_OF_MEM;
    }

    ushd_log(LOG_LVL_DETAIL, "sending ack msg %p ......", ack);

    ush_ret_t ret = ushd_dist_thread_send_msg(
        thread, ack,
        ush_comm_lstnr_sigreg_ack_sizeof(),
        USH_COMM_LSTNR_SEND_PRIO_SIGREG_ACK);

    if (USH_RET_OK != ret) {
        ushd_log(LOG_LVL_ERROR, "send hay %p to lstnr failed.", ack);
    }
    ush_comm_lstnr_sigreg_ack_destroy(&ack);
    return ret;
}


ush_ret_t
ushd_dist_proc_sig_send_update(ushd_dist_thread_t thread,
                               const dist_fifo_msg_sig_upd *msg) {
    if (!thread || !msg) {
        ushd_log(LOG_LVL_ERROR, "null ptr of param");
        return USH_RET_WRONG_PARAM;
    }

    ush_comm_lstnr_sig_upd_t upd =
        ush_comm_lstnr_sig_upd_create(msg->sigid, msg->val, msg->rcv);
    if (!upd) {
        ushd_log(LOG_LVL_ERROR, "update msg mem alloc failed.");
        return USH_RET_OUT_OF_MEM;
    }

    ushd_log(LOG_LVL_DETAIL, "sending msg %p in thread %p......", upd, thread);

    ush_ret_t ret = ushd_dist_thread_send_msg(
        thread, upd,
        ush_comm_lstnr_sig_upd_sizeof(),
        USH_COMM_LSTNR_SEND_PRIO_SIG_UPD);

    if (USH_RET_OK != ret) {
        ushd_log(LOG_LVL_ERROR, "send upd %p to lstnr failed.", upd);
    }
    ush_comm_lstnr_sig_upd_destroy(&upd);
    return ret;
}
