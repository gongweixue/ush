#include "string.h"

#include "ush_assert.h"
#include "ush_log.h"

#include "conn/ushd_conn_tbl.h"
#include "conn/ushd_conn_reglist.h"

#include "ush_comm_desc.h"
#include "realm/ush_comm_realm.h"
#include "realm/sig/ush_comm_realm_sig.h"
#include "realm/sig/ush_comm_realm_sigreg.h"
#include "realm/sig/ush_comm_realm_sigset.h"

#include "dist/ushd_dist_fifo.h"
#include "dist/ushd_dist_fifo_msg.h"

#include "ushd_sched_proc_realm_sig.h"

static void ushd_sched_proc_realm_sigreg(const ush_comm_realm_sigreg_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "NULL reg msg");
        return;
    }

    // check idx
    ush_connidx_t idx = ush_comm_realm_sigreg_get_connidx(msg);
    if (!ushd_conn_tbl_get_active_flg(idx)) {
        ushd_log(LOG_LVL_ERROR, "Invalid idx:%d value of tbl", idx);
        return;
    }

    // check cert
    const ush_cert_t cert = ush_comm_realm_sigreg_get_cert(msg);
    const ush_cert_t ref  = ushd_conn_tbl_get_cert(idx);
    if (ref != cert) {
        ushd_log(LOG_LVL_ERROR, "cert:%d, idx:%d, ref:%d", cert, ref, idx);
        return;
    }

    // check sigid valid
    ush_sig_id_t sigid = ush_comm_realm_sigreg_get_sigid(msg);
    if (!ush_sig_id_check(sigid)) {
        ushd_log(LOG_LVL_ERROR, "Invalid sigid:%d", sigid);
        return;
    }

    // ack for
    dist_fifo_msg_sigreg_ack ack;
    ack.desc.type = USHD_DIST_FIFO_MSG_TYPE_SIGREG_ACK;
    ack.success   = USH_FALSE;
    ack.sigid     = sigid;
    ack.pipe      = ush_comm_realm_sigreg_get_pipe(msg);
    ack.done      = ush_comm_realm_sigreg_get_cb_done(msg);

    // add cb func to the sigid domain of conn in the idx.
    ush_pvoid_t rcv  = ush_comm_realm_sigreg_get_cb_rcv(msg);
    if (USH_RET_OK != ushd_conn_reglist_set_rcv(idx, sigid, rcv)) {
        ack.success = USH_FALSE;
        ushd_log(LOG_LVL_ERROR, "callback register failed");
    } else {
        ack.success = 1;
    }

    ushd_dist_thread_t dist = ushd_conn_tbl_get_dist(idx);
    if (!dist) {
        ushd_log(LOG_LVL_ERROR, "dist thread of %d is NULL", idx);
        return;
    }
    ushd_dist_fifo_t fifo = ushd_dist_thread_get_fifo(dist);
    if (!fifo) {
        ushd_log(LOG_LVL_ERROR, "no dist fifo of connidx %d to push", idx);
        return;
    }
    ushd_dist_fifo_push(fifo, (dist_fifo_msg_d*)&ack, sizeof(ack));

    return;
}


static void notify_handle(ush_connidx_t connidx,
                          ush_sig_id_t  sigid,
                          ush_sig_val_t val,
                          ush_pvoid_t   rcv) {
    if (!ushd_conn_tbl_get_active_flg(connidx)) {
        ushd_log(LOG_LVL_INFO, "inactive idx:%d value of tbl", connidx);
        return;
    }

    dist_fifo_msg_sig_upd msg;
    msg.desc.type   = USHD_DIST_FIFO_MSG_TYPE_SIG_UPD;
    msg.sigid       = sigid;
    msg.val.dataMAX = val.dataMAX;
    msg.rcv         = rcv;

    ushd_dist_thread_t dist = ushd_conn_tbl_get_dist(connidx);
    if (!dist) {
        ushd_log(LOG_LVL_ERROR, "dist thread of %d is NULL", connidx);
        return;
    }
    ushd_dist_fifo_t fifo = ushd_dist_thread_get_fifo(dist);
    if (!fifo) {
        ushd_log(LOG_LVL_ERROR, "no dist fifo of connidx %d to push", connidx);
        return;
    }
    ushd_dist_fifo_push(fifo, (dist_fifo_msg_d*)&msg, sizeof(msg));

    return;
}
static void ushd_sched_proc_realm_sigset(const ush_comm_realm_sigset_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "NULL sig-set msg");
        return;
    }

    // check idx
    ush_connidx_t idx = ush_comm_realm_sigset_get_connidx(msg);
    if (!ushd_conn_tbl_get_active_flg(idx)) {
        ushd_log(LOG_LVL_ERROR, "Invalid idx:%d value of tbl", idx);
        return;
    }

    // check cert
    const ush_cert_t cert = ush_comm_realm_sigset_get_cert(msg);
    const ush_cert_t ref  = ushd_conn_tbl_get_cert(idx);
    if (ref != cert) {
        ushd_log(LOG_LVL_ERROR, "cert:%d, idx:%d, ref:%d", cert, ref, idx);
        return;
    }

    // check sigid valid
    ush_sig_id_t sigid = ush_comm_realm_sigset_get_sigid(msg);
    if (!ush_sig_id_check(sigid)) {
        ushd_log(LOG_LVL_ERROR, "Invalid sigid:%d", sigid);
        return;
    }

    // update the sig value in reglist
    ush_sig_val_t val = ush_comm_realm_sigset_get_val(msg);
    if (USH_RET_OK != ushd_conn_reglist_cas(sigid, val)) {
        ushd_log(LOG_LVL_INFO, "signal %d value update failed", sigid);
        return;
    }
    if (USH_RET_OK != ushd_conn_reglist_notify(sigid, notify_handle)) {
        ushd_log(LOG_LVL_ERROR, "notify sigid %d update failed", sigid);
        return;
    }

    return;
}

void ushd_sched_proc_realm_sig(const ush_pvoid_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "msg is NULL!!!");
        return;
    }

    const ush_comm_realm_sig_d *pDesc = (const ush_comm_realm_sig_d *)msg;

    switch (pDesc->intent) {
    case USH_COMM_REALM_SIG_INTENT_REG:
        ushd_sched_proc_realm_sigreg((const ush_comm_realm_sigreg_t)msg);
        break;

    case USH_COMM_REALM_SIG_INTENT_SET:
        ushd_sched_proc_realm_sigset((const ush_comm_realm_sigset_t)msg);
        break;

    default:
        ushd_log(LOG_LVL_FATAL, "wrong intent #%d", pDesc->intent);
        break;
    }
}
