
#include "stdlib.h"

#include "ush_log.h"
#include "tch/ush_comm_tch.h"
#include "tch/sig/ush_comm_tch_sig.h"
#include "tch/sig/ush_comm_tch_sig_reg.h"


typedef struct comm_tch_sig_reg {
    ush_comm_tch_sig_d           desc;
    ush_s32_t                    remote_idx;
    ush_s32_t                    cert;
    ush_sig_id_t                 sig;
    ush_pvoid_t                  done;
    ush_pvoid_t                  rcv;
    ush_pipe_t                   pipe;
} USH_COMM_MSG_PACKED  * ush_comm_tch_sig_reg_t;


ush_ret_t
ush_comm_tch_sig_reg_create(ush_comm_tch_sig_reg_t *pMsg,
                            ush_s32_t               idxconn,
                            ush_s32_t               cert,
                            ush_sig_id_t            sigid,
                            const ush_pvoid_t       done,
                            const ush_pvoid_t       rcv,
                            ush_pipe_t              pipe)
{
    if (!pMsg) {
        ush_log(LOG_LVL_ERROR, "ptr NULL");
        return USH_RET_WRONG_PARAM;
    }

    ush_comm_tch_sig_reg_t tmp =
        (ush_comm_tch_sig_reg_t)malloc(sizeof(struct comm_tch_sig_reg));
    if (!tmp) {
        ush_log(LOG_LVL_ERROR, "out of mem");
        return USH_RET_OUT_OF_MEM;
    }

    tmp->desc.desc.desc.port = USH_COMM_PORT_TCH;
    tmp->desc.desc.catalog   = USH_COMM_TCH_MSG_CATALOG_SIG;
    tmp->desc.intent         = USH_COMM_TCH_SIG_INTENT_REG;
    tmp->remote_idx          = idxconn;
    tmp->cert                = cert;
    tmp->sig                 = sigid;
    tmp->done                = done;
    tmp->rcv                 = rcv;
    tmp->pipe                = pipe;

    *pMsg = tmp;

    return USH_RET_OK;
}

ush_ret_t ush_comm_tch_sig_reg_destroy(ush_comm_tch_sig_reg_t *pMsg) {
    if (!pMsg) {
        return USH_RET_OK;
    }

    ush_comm_tch_sig_reg_t msg = *pMsg;
    if (!msg) {
        return USH_RET_OK;
    }

    free(msg);
    *pMsg = NULL;

    return USH_RET_OK;
}
