
#include "tch/ush_comm_tch.h"
#include "tch/sig/ush_comm_tch_sig.h"
#include "tch/sig/ush_comm_tch_sig_reg.h"


typedef struct comm_tch_sig_reg {
    ush_comm_tch_sig_d           desc;
    ush_s32_t                    remote_idx;
    ush_s32_t                    cert;
    ush_sig_id_t                 sig;
    ush_sig_cb_reg_t             done;
    ush_sig_cb_rcv_t             rcv;
    ush_pipe_t                   pipe;
} USH_COMM_MSG_PACKED  * ush_comm_tch_sig_reg_t;


ush_ret_t
ush_comm_tch_sig_reg_create(ush_comm_tch_sig_reg_t *msg,
                            ush_s32_t               idxconn,
                            ush_s32_t               cert,
                            ush_sig_id_t            sigid,
                            const void             *done,
                            const void             *rcv,
                            ush_pipe_t              pipe)
{
    // ush_assert(msg && *msg && idx != 0 && cert != INVALD && pipe != 0);
    return USH_RET_OK;
}

ush_ret_t ush_comm_tch_sig_reg_destroy(ush_comm_tch_sig_reg_t *msg) {
    return USH_RET_OK;
}
