#include "stdlib.h"

#include "ush_log.h"
#include "ush_comm_lstnr.h"
#include "ush_comm_lstnr_sig_upd.h"


typedef struct lstnr_sig_upd_s {
    ush_comm_lstnr_msg_d         desc;
    ush_sig_id_t                 sigid;
    ush_sig_val_t                val;
    ush_pvoid_t                  rcv;
    ush_u32_t                    cntr;
} USH_COMM_MSG_PACKED * ush_comm_lstnr_sig_upd_t;

ush_comm_lstnr_sig_upd_t
ush_comm_lstnr_sig_upd_create(ush_sig_id_t  sigid,
                              ush_sig_val_t val,
                              ush_pvoid_t   rcv,
                              ush_u32_t     cntr) {
    ush_comm_lstnr_sig_upd_t ret =
        (ush_comm_lstnr_sig_upd_t)malloc(sizeof(struct lstnr_sig_upd_s));

    if (!ret) {
        ush_log(LOG_LVL_ERROR, "sig_upd msg mem alloc failed.");
        return NULL;
    }

    ret->desc.desc.port = USH_COMM_PORT_LSTNR;
    ret->desc.catalog   = USH_COMM_LSTNR_MSG_CATALOG_SIG_UPD;
    ret->sigid          = sigid;
    ret->val.dataMAX    = val.dataMAX;
    ret->rcv            = rcv;
    ret->cntr           = cntr;

    return ret;
}

ush_size_t
ush_comm_lstnr_sig_upd_sizeof(void) {
    return sizeof(struct lstnr_sig_upd_s);
}

ush_sig_cb_rcv_t
ush_comm_lstnr_sig_callback_of(const ush_comm_lstnr_sig_upd_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "msg ptr null");
        return NULL;
    }

    return msg->rcv;
}

ush_sig_val_t
ush_comm_lstnr_sig_value_of(const ush_comm_lstnr_sig_upd_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "msg ptr null");
        ush_sig_val_t ret;
        ret.dataMAX = 0;
        return ret;
    }

    return msg->val;
}

ush_sig_id_t
ush_comm_lstnr_sig_sigid_of(const ush_comm_lstnr_sig_upd_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "msg ptr null");
        return USH_SIG_ID_INVALID;
    }

    return msg->sigid;
}

ush_u32_t
ush_comm_lstnr_sig_cntr_of(const ush_comm_lstnr_sig_upd_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "msg ptr null");
        return 0;
    }

    return msg->cntr;
}

ush_ret_t
ush_comm_lstnr_sig_upd_destroy(ush_comm_lstnr_sig_upd_t *pMsg) {
    if (!pMsg) {
        ush_log(LOG_LVL_INFO, "msg ptr null");
        return USH_RET_OK;
    }

    ush_comm_lstnr_sig_upd_t msg = *pMsg;
    if (!msg) {
        ush_log(LOG_LVL_INFO, "msg ptr null");
        return USH_RET_OK;
    }

    free(*pMsg);
    *pMsg = NULL;
    return USH_RET_OK;
}

void
ush_comm_lstnr_sig_upd_testpoint(const ush_comm_lstnr_sig_upd_t msg) {
    (void)msg;
}
