#include "ush_assert.h"
#include "ush_log.h"

#include "conn/ushd_conn_tbl.h"
#include "conn/ushd_conn_reglist.h"

#include "ush_comm_desc.h"
#include "tch/ush_comm_tch.h"
#include "tch/sig/ush_comm_tch_sig.h"
#include "tch/sig/ush_comm_tch_sig_reg.h"

#include "dist/ushd_dist_fifo.h"
#include "dist/ushd_dist_fifo_msg.h"
#include "ushd_sched_proc_tch_sig.h"

static void ushd_sched_proc_tch_sig_reg(const ush_comm_tch_sig_reg_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "NULL reg msg");
        return;
    }

    // check idx
    ush_connidx_t idx = ush_comm_tch_sig_reg_get_connidx(msg);
    if (!ushd_conn_tbl_get_valid(idx)) {
        ushd_log(LOG_LVL_ERROR, "Invalid idx:%d value of tbl", idx);
        return;
    }

    // check cert
    const ush_cert_t cert = ush_comm_tch_sig_reg_get_cert(msg);
    const ush_cert_t ref  = ushd_conn_tbl_get_cert(idx);
    if (ref != cert) {
        ushd_log(LOG_LVL_ERROR, "cert:%d, idx:%d, ref:%d", cert, ref, idx);
        return;
    }

    // check sigid valid
    ush_sig_id_t sigid = ush_comm_tch_sig_reg_get_sigid(msg);
    if (!ush_sig_id_valid(sigid)) {
        ushd_log(LOG_LVL_ERROR, "Invalid sigid:%d", sigid);
        return;
    }

    // ack for
    dist_fifo_msg_sig_reg_ack ack;
    ack.desc.type = USHD_DIST_FIFO_MSG_TYPE_SIG_REG_ACK;
    ack.success   = 0;
    ack.sigid     = sigid;
    ack.pipe      = ush_comm_tch_sig_reg_get_pipe(msg);
    ack.done      = ush_comm_tch_sig_reg_get_cb_done(msg);

    // add cb func to the sigid domain of conn in the idx.
    ush_pvoid_t rcv  = ush_comm_tch_sig_reg_get_cb_rcv(msg);
    if (USH_RET_OK != ushd_conn_reglist_set_rcv(idx, sigid, rcv)) {
        ack.success = 0;
        ushd_log(LOG_LVL_ERROR, "callback register failed");
    } else {
        ack.success = 1;
    }

    ushd_dist_thread_t dist = ushd_conn_tbl_get_dist(idx);
    ushd_dist_fifo_t   fifo = ushd_dist_thread_get_fifo(dist);
    if (!fifo) {
        ushd_log(LOG_LVL_ERROR, "no dist fifo to push");
    }
    ushd_dist_fifo_push(fifo, (dist_fifo_msg_d*)&ack, sizeof(ack));

    return;
}

void ushd_sched_proc_tch_sig(const ush_pvoid_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "msg is NULL!!!");
        return;
    }

    const ush_comm_tch_sig_d *pDesc = (const ush_comm_tch_sig_d *)msg;

    switch (pDesc->intent) {
    case USH_COMM_TCH_SIG_INTENT_REG:
        ushd_sched_proc_tch_sig_reg((const ush_comm_tch_sig_reg_t)msg);
        break;

    default:
        ushd_log(LOG_LVL_FATAL, "wrong intent #%d", pDesc->intent);
        break;
    }
}
