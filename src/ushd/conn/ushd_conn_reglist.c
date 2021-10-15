#include "pthread.h"

#include "ush_log.h"
#include "ush_type_pub.h"
#include "ush_sig_enum.h"

#include "ushd_conn_tbl.h"
#include "ushd_conn_reglist.h"

//////////////////////////////////////////////////////////////////////
// CAUTION: all member of reglist are static, and be initialized by 0.
//////////////////////////////////////////////////////////////////////

typedef union ush_reglist_sig_latest_val_t {
    BOOL    dataBOOL;
    U8      dataU8;
    S8      dataS8;
    U16     dataU16;
    S16     dataS16;
    U32     dataU32;
    S32     dataS32;
    U64     dataU64;
    S64     dataS64;
    FP32    dataFP32;
    FP64    dataFP64;
    CHAR    dataCHAR;
} ush_reglist_sig_latest_val_t;

typedef struct ush_reglist_sig_node_t {
    ush_pvoid_t    rcv;
} ush_reglist_sig_node_t;


typedef struct ush_reglist_sig_ty {
    ush_reglist_sig_node_t  node[USH_CONN_IDX_MAX];
} ush_reglist_sig_ty;

typedef struct ush_reglist_t {
    pthread_mutex_t         creticalsection;
    ush_reglist_sig_ty      signals[USH_SIG_ID_MAX];
} ush_reglist_t;

static ush_reglist_t reglist;

ush_ret_t ushd_conn_reglist_init() {
    static ush_bool_t flag = 0;
    if (1 == flag) {
        ushd_log(LOG_LVL_INFO, "reglist has been alreay init.");
        return USH_RET_OK;
    }

    pthread_mutex_init(&reglist.creticalsection, NULL);

    flag = 1;
    return USH_RET_OK;
}

static inline void reglist_cs_entry() {
    pthread_mutex_lock(&(reglist.creticalsection));
}
static inline void reglist_cs_exit() {
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
