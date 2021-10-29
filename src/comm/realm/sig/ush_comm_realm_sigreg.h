#ifndef USH_COMM_REALM_SIGREG_H
#define USH_COMM_REALM_SIGREG_H

#include "ush_type_pub.h"
#include "ush_sig_id.h"

typedef struct comm_realm_sigreg_s * ush_comm_realm_sigreg_t;


ush_ret_t
ush_comm_realm_sigreg_create(ush_comm_realm_sigreg_t  *pMsg,
                             ush_connidx_t           connidx,
                             ush_cert_t              cert,
                             ush_sig_id_t            sigid,
                             const ush_pvoid_t       done,
                             const ush_pvoid_t       rcv,
                             ush_pipe_t              pipe);

ush_cert_t
ush_comm_realm_sigreg_get_cert(const ush_comm_realm_sigreg_t msg);

ush_connidx_t
ush_comm_realm_sigreg_get_connidx(const ush_comm_realm_sigreg_t msg);

ush_sig_id_t
ush_comm_realm_sigreg_get_sigid(const ush_comm_realm_sigreg_t msg);

ush_pvoid_t
ush_comm_realm_sigreg_get_cb_done(const ush_comm_realm_sigreg_t msg);

ush_pvoid_t
ush_comm_realm_sigreg_get_cb_rcv(const ush_comm_realm_sigreg_t msg);

ush_pipe_t
ush_comm_realm_sigreg_get_pipe(const ush_comm_realm_sigreg_t msg);

ush_size_t
ush_comm_realm_sigreg_sizeof(void);

ush_ret_t
ush_comm_realm_sigreg_destroy(ush_comm_realm_sigreg_t *msg);

void
ush_comm_realm_sigreg_testpoint(const ush_comm_realm_sigreg_t msg);

#endif // USH_COMM_REALM_SIGREG_H
