#include "pthread.h"
#include "string.h"

#include "ush_log.h"
#include "ush_type_pub.h"
#include "ush_sig_id.h"

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
    ush_bool_t              valid; // is current value valid to use
} ush_reglist_sig_ty;

typedef struct ush_reglist_s {
    ush_reglist_sig_ty      signals[USH_SIG_ID_MAX];
} ush_reglist_t;

static ush_reglist_t reglist; // static var, all 0 while loading.

ush_ret_t ushd_conn_reglist_init(void) {
    static ush_bool_t flag = USH_FALSE;
    if (USH_TRUE == flag) {
        ushd_log(LOG_LVL_INFO, "reglist has been alreay init.");
        return USH_RET_OK;
    }

    flag = USH_TRUE;
    return USH_RET_OK;
}

ush_ret_t
ushd_conn_reglist_set_rcv(ush_connidx_t idx,
                          ush_sig_id_t  sigid,
                          ush_pvoid_t   rcv) {
    if (!ushd_conn_tbl_connidx_check(idx) || !ush_sig_id_check(sigid)) {
        ushd_log(LOG_LVL_ERROR, "idx or sigid is out of bound");
        return USH_RET_FAILED;
    }

    reglist.signals[sigid].nodes[idx].rcv = rcv;

    return USH_RET_OK;
}

ush_ret_t
ushd_conn_reglist_cas(ush_sig_id_t sigid, ush_sig_val_t val) {
    if (!ush_sig_id_check(sigid)) {
        ushd_log(LOG_LVL_ERROR, "idx or sigid is out of bound");
        return USH_RET_FAILED;
    }

    ush_ret_t ret = USH_RET_FAILED;
    if (1 != reglist.signals[sigid].valid) { // first time to assigned
        reglist.signals[sigid].value.dataMAX = val.dataMAX;
        reglist.signals[sigid].valid = USH_TRUE;
        ret = USH_RET_OK;
    } else {
        if (val.dataMAX != reglist.signals[sigid].value.dataMAX) { // new value
            reglist.signals[sigid].value.dataMAX = val.dataMAX;
            ret = USH_RET_OK;
        } else { // same value, no need to update, return failed.
            ret = USH_RET_FAILED;
        }
    }
    return ret;
}

ush_ret_t
ushd_conn_reglist_notify(ush_sig_id_t sigid, notify_func_t func) {
    if (!func || !ush_sig_id_check(sigid)) {
        return USH_RET_WRONG_PARAM;
    }

    if (0 == reglist.signals[sigid].valid) { // signal is meaningful
        return USH_RET_FAILED;
    }

    ushd_log(LOG_LVL_DETAIL, "begin sending notify to listeners");
    // cb to notify all observers.
    ush_sig_val_t           val   = reglist.signals[sigid].value;
    ush_reglist_sig_node_t *nodes = reglist.signals[sigid].nodes;
    for (ush_connidx_t connidx= 0; connidx < USH_CONN_IDX_MAX; ++connidx) {
        if (!ushd_conn_tbl_get_active_flg(connidx) || !nodes[connidx].rcv) {
            continue;
        }

        func(connidx, sigid, val, nodes[connidx].rcv);
    }

    return USH_RET_OK;
}
