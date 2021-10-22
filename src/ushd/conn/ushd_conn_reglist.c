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

typedef struct ush_reglist_sig_node_t {
    ush_pvoid_t    rcv;
} ush_reglist_sig_node_t;


typedef struct ush_reglist_sig_ty {
    ush_reglist_sig_node_t  node[USH_CONN_IDX_MAX];
    ush_sig_val_t           value; // current value of the signal
    ush_bool_t              valid; // is current value valid to use
} ush_reglist_sig_ty;

typedef struct ush_reglist_t {
    pthread_mutex_t         creticalsection;
    ush_reglist_sig_ty      signals[USH_SIG_ID_MAX];
} ush_reglist_t;

static ush_reglist_t reglist; // static var, all 0 while loading.

ush_ret_t ushd_conn_reglist_init(void) {
    static ush_bool_t flag = 0;
    if (1 == flag) {
        ushd_log(LOG_LVL_INFO, "reglist has been alreay init.");
        return USH_RET_OK;
    }

    pthread_mutex_init(&reglist.creticalsection, NULL);

    flag = 1;
    return USH_RET_OK;
}

static inline void reglist_cs_entry(void) {
    pthread_mutex_lock(&(reglist.creticalsection));
}
static inline void reglist_cs_exit(void) {
    pthread_mutex_unlock(&(reglist.creticalsection));
}

ush_ret_t
ushd_conn_reglist_set_rcv(ush_connidx_t idx,
                          ush_sig_id_t  sigid,
                          ush_pvoid_t   rcv) {
    if (!ush_conn_tbl_connidx_valid(idx) || !ush_sig_id_valid(sigid)) {
        ushd_log(LOG_LVL_ERROR, "idx or sigid is out of bound");
        return USH_RET_FAILED;
    }

    reglist_cs_entry();
    reglist.signals[sigid].node[idx].rcv = rcv;
    reglist_cs_exit();

    return USH_RET_OK;
}

ush_ret_t
ushd_conn_reglist_set_val(ush_connidx_t idx,
                          ush_sig_id_t  sigid,
                          ush_sig_val_t val) {
    if (!ush_conn_tbl_connidx_valid(idx) || !ush_sig_id_valid(sigid)) {
        ushd_log(LOG_LVL_ERROR, "idx or sigid is out of bound");
        return USH_RET_FAILED;
    }

    reglist_cs_entry();
    memcpy(&(reglist.signals[sigid].value), &val, sizeof(val));
    reglist.signals[sigid].valid = 1;
    reglist_cs_exit();

    return USH_RET_OK;
}
