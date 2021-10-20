#ifndef USH_COMM_TCH_SIG_SET_H
#define USH_COMM_TCH_SIG_SET_H

#include "ush_type_pub.h"

typedef struct comm_tch_sig_set * ush_comm_tch_sig_set_t;

ush_ret_t
ush_comm_tch_sig_set_create(ush_comm_tch_sig_set_t *pMsg,
                            ush_connidx_t           connidx,
                            ush_cert_t              cert,
                            ush_sig_id_t            sigid,
                            ush_sig_val_t           val,
                            ush_pipe_t              pipe);

ush_connidx_t
ush_comm_tch_sig_set_get_connidx(const ush_comm_tch_sig_set_t msg);

ush_cert_t
ush_comm_tch_sig_set_get_cert(const ush_comm_tch_sig_set_t msg);

ush_sig_id_t
ush_comm_tch_sig_set_get_sigid(const ush_comm_tch_sig_set_t msg);

ush_sig_val_t
ush_comm_tch_sig_set_get_val(const ush_comm_tch_sig_set_t msg);

ush_pipe_t
ush_comm_tch_sig_set_get_pipe(const ush_comm_tch_sig_set_t msg);

ush_size_t
ush_comm_tch_sig_set_sizeof(void);

ush_ret_t
ush_comm_tch_sig_set_destroy(ush_comm_tch_sig_set_t *msg);

#endif // USH_COMM_TCH_SIG_SET_H
