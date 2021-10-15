#ifndef USH_COMM_TCH_SIG_REG_H
#define USH_COMM_TCH_SIG_REG_H

#include "ush_type_pub.h"

typedef struct comm_tch_sig_reg * ush_comm_tch_sig_reg_t;


ush_ret_t
ush_comm_tch_sig_reg_create(ush_comm_tch_sig_reg_t *pMsg,
                            ush_s32_t               idxconn,
                            ush_s32_t               cert,
                            ush_sig_id_t            sigid,
                            const ush_pvoid_t       done,
                            const ush_pvoid_t       rcv,
                            ush_pipe_t              pipe);

ush_s32_t
ush_comm_tch_sig_reg_get_cert(ush_comm_tch_sig_reg_t msg);

ush_s32_t
ush_comm_tch_sig_reg_get_remote_idx(ush_comm_tch_sig_reg_t msg);

ush_sig_id_t
ush_comm_tch_sig_reg_get_sigid(ush_comm_tch_sig_reg_t msg);

ush_pvoid_t
ush_comm_tch_sig_reg_get_cb_done(ush_comm_tch_sig_reg_t msg);

ush_pvoid_t
ush_comm_tch_sig_reg_get_cb_rcv(ush_comm_tch_sig_reg_t msg);

ush_pipe_t
ush_comm_tch_sig_reg_get_pipe(ush_comm_tch_sig_reg_t msg);

ush_size_t
ush_comm_tch_sig_reg_sizeof();

ush_ret_t
ush_comm_tch_sig_reg_destroy(ush_comm_tch_sig_reg_t *msg);

#endif // USH_COMM_TCH_SIG_REG_H
