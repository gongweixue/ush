
#include "ush_define.h"
#include "ush_log.h"
#include "ush_type_pub.h"
#include "ush_sigid_pub.h"

#include "ushd_conn_tbl.h"
#include "ushd_conn_reglist.h"

//////////////////////////////////////////////////////////////////////
// CAUTION: all member of reglist are static, and be initialized by 0.
//////////////////////////////////////////////////////////////////////

typedef struct ush_reglist_sig_node_s {
    ush_pvoid_t    rcv;
} ush_reglist_sig_node_t;


typedef struct ush_reglist_sig_s {
    ush_reglist_sig_node_t  nodes[USH_CONN_IDX_MAX];
    ush_sig_val_t           value; // current value of the signal
    ush_u32_t               rollingcounter; // increse by 1 each time
} ush_reglist_sig_ty;

typedef struct ush_reglist_s {
    ush_reglist_sig_ty      signals[USH_SIG_ID_MAX];
} ush_reglist_t;

static ush_reglist_t reglist; // static var, all 0 while loading.

ush_ret_t ushd_conn_reglist_init(void) {
    static ush_bool_t flag = USH_FALSE;
    if (USH_FALSE != flag) {
        ushd_log(LOG_LVL_INFO, "reglist has been alreay init.");
        return USH_RET_OK;
    }

    flag = USH_TRUE;
    return USH_RET_OK;
}

ush_ret_t
ushd_conn_reglist_set_rcv(ush_connidx_t idx,ush_sigid_t sigid,ush_pvoid_t rcv) {
    if (!ushd_conn_tbl_connidx_check(idx) || !ush_sigid_check(sigid)) {
        ushd_log(LOG_LVL_ERROR, "idx or sigid is out of bound");
        return USH_RET_FAILED;
    }

    reglist.signals[sigid].nodes[idx].rcv = rcv;

    return USH_RET_OK;
}

ush_ret_t
ushd_conn_reglist_cas(ush_sigid_t sigid, ush_sig_val_t val) {
    if (!ush_sigid_check(sigid)) {
        ushd_log(LOG_LVL_ERROR, "idx or sigid is out of bound");
        return USH_RET_FAILED;
    }

    ush_ret_t ret = USH_RET_FAILED;
    if (val.dataMAX != reglist.signals[sigid].value.dataMAX ||
        0 == reglist.signals[sigid].rollingcounter) { // new-value/first-assign
        reglist.signals[sigid].value.dataMAX = val.dataMAX;
        reglist.signals[sigid].rollingcounter += 1;
        ret = USH_RET_OK;
    } else {
        ret = USH_FALSE;
    }

    return ret;
}

// notify all observers of this sigid if idx == USHD_INVALID_CONN_IDX_VALUE
ush_ret_t
ushd_conn_reglist_notify(ush_sigid_t   sigid,
                         ush_connidx_t connidx,
                         notify_func_t func) {
    if (!func || !ush_sigid_check(sigid)) {
        return USH_RET_WRONG_PARAM;
    }

    if (0 == reglist.signals[sigid].rollingcounter) { // sig must be meaningful
        return USH_RET_FAILED;
    }

    ushd_log(LOG_LVL_INFO, "begin sending notify to listeners");
    // cb to notify all observers.
    ush_sig_val_t           val     = reglist.signals[sigid].value;
    ush_u32_t               counter = reglist.signals[sigid].rollingcounter;
    ush_reglist_sig_node_t *nodes   = reglist.signals[sigid].nodes;

    // for any single conn
    if (ushd_conn_tbl_get_active_flg(connidx)) {
        if (nodes[connidx].rcv) { // has a receive callback
            func(connidx, sigid, val, nodes[connidx].rcv, counter);
            ushd_log(LOG_LVL_INFO, "notify single observer, sig:%d", sigid);
        }

    } else if (USHD_INVALID_CONN_IDX_VALUE == connidx) { // for all observers
        for (ush_connidx_t idx= 0; idx < USH_CONN_IDX_MAX; ++idx) {
            if (!ushd_conn_tbl_get_active_flg(idx) || !nodes[idx].rcv) {
                continue;
            }

            func(idx, sigid, val, nodes[idx].rcv, counter);
        }
    }



    return USH_RET_OK;
}
