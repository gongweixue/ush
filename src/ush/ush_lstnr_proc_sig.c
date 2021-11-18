#include "ush_log.h"
#include "ush_type_pub.h"
#include "ush_sigid_pub.h"
#include "ush_lstnr_proc_sig.h"


void ush_lstnr_proc_sigreg_ack(const ush_comm_lstnr_sigreg_ack_t msg) {
    ush_sig_cb_reg_t done_cb = ush_comm_lstnr_sigreg_ack_callback_of(msg);
    if (!done_cb) {
            return; // nothing to do
    }

    ush_pipe_t            pipe = ush_comm_lstnr_sigreg_ack_pipe_of(msg);
    const ush_sigid_t  *pSigid = ush_comm_lstnr_sigreg_ack_sigid_of(msg);
    const ush_bool_t *pSuccess = ush_comm_lstnr_sigreg_ack_succ_of(msg);

    ush_log(LOG_LVL_INFO,
            "'registe done' callback invoke, pipe:%llu, sigid:%p, succ:%p",
            pipe, pSigid, pSuccess);

    done_cb(pipe, pSigid, pSuccess);
}


void ush_lstnr_proc_sig_upd(const ush_comm_lstnr_sig_upd_t msg) {
    ush_sig_cb_rcv_t rcv_cb = ush_comm_lstnr_sig_callback_of(msg);
    if (!rcv_cb) {
            return; // nothing to do
    }

    ush_sigid_t   sigid = ush_comm_lstnr_sig_sigid_of(msg);
    ush_sig_val_t value = ush_comm_lstnr_sig_value_of(msg);
    ush_u32_t   version = ush_comm_lstnr_sig_version_of(msg);

    rcv_cb(sigid, value, version);
}
