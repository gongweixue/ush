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

ush_size_t
ush_comm_tch_sig_reg_sizeof();

ush_ret_t
ush_comm_tch_sig_reg_destroy(ush_comm_tch_sig_reg_t *msg);

#endif // USH_COMM_TCH_SIG_REG_H
