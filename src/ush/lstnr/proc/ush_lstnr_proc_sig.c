#include "ush_log.h"

#include "ush_cb_pub.h"
#include "ush_lstnr_proc_sig.h"


void ush_lstnr_proc_sigreg_ack(const ush_comm_lstnr_sigreg_ack_t msg) {
    ush_sig_cb_reg_t done_cb = ush_comm_lstnr_sigreg_ack_get_done(msg);
    if (!done_cb) {
            return; // nothing to do
    }

    ush_pipe_t    pipe = ush_comm_lstnr_sigreg_ack_get_pipe(msg);
    ush_sig_id_t sigid = ush_comm_lstnr_sigreg_ack_get_sigid(msg);
    ush_bool_t success = ush_comm_lstnr_sigreg_ack_get_success(msg);

    ush_log(LOG_LVL_INFO,
            "'registe done' callback invoke, pipe:%llu, sigid:%d, succ:%d",
            pipe, sigid, success);

    done_cb(pipe, sigid, success);
}


void ush_lstnr_proc_sig_upd(const ush_comm_lstnr_sig_upd_t msg) {
    ush_sig_cb_rcv_t rcv_cb = ush_comm_lstnr_sig_upd_get_rcv(msg);
    if (!rcv_cb) {
            return; // nothing to do
    }

    ush_sig_id_t  sigid = ush_comm_lstnr_sig_upd_get_sigid(msg);
    ush_sig_val_t value = ush_comm_lstnr_sig_upd_get_value(msg);

    ush_u32_t     rollingcounter = ush_comm_lstnr_sig_upd_get_cntr(msg);

    rcv_cb(sigid, value, rollingcounter);
}
