#ifndef USH_COMM_REALM_SIGSET_H
#define USH_COMM_REALM_SIGSET_H

#include "ush_type_pub.h"

typedef struct comm_realm_sigset_s * ush_comm_realm_sigset_t;

ush_ret_t
ush_comm_realm_sigset_create(ush_comm_realm_sigset_t  *pMsg,
                             ush_connidx_t             connidx,
                             ush_cert_t                cert,
                             ush_sig_id_t              sigid,
                             ush_sig_val_t             val,
                             ush_pipe_t                pipe);

ush_connidx_t
ush_comm_realm_sigset_get_connidx(const ush_comm_realm_sigset_t msg);

ush_cert_t
ush_comm_realm_sigset_get_cert(const ush_comm_realm_sigset_t msg);

ush_sig_id_t
ush_comm_realm_sigset_get_sigid(const ush_comm_realm_sigset_t msg);

ush_sig_val_t
ush_comm_realm_sigset_get_val(const ush_comm_realm_sigset_t msg);

ush_pipe_t
ush_comm_realm_sigset_get_pipe(const ush_comm_realm_sigset_t msg);

ush_size_t
ush_comm_realm_sigset_sizeof(void);

ush_ret_t
ush_comm_realm_sigset_destroy(ush_comm_realm_sigset_t *msg);

#endif // USH_COMM_REALM_SIGSET_H
