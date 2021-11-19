#ifndef USH_COMM_REALM_SIGREG_H
#define USH_COMM_REALM_SIGREG_H

#include "ush_type_pub.h"
#include "ush_sig_pub.h"
#include "ush_def_pub.h"

typedef struct comm_realm_sigreg_s * ush_comm_realm_sigreg_t;


ush_ret_t
ush_comm_realm_sigreg_create(ush_comm_realm_sigreg_t  *pMsg,
                             ush_connidx_t             connidx,
                             ush_cert_t                cert,
                             ush_pipe_t                pipe,
                             const ush_sigreg_conf_t  *pConf);

ush_cert_t
ush_comm_realm_sigreg_cert_of(const ush_comm_realm_sigreg_t msg);

ush_connidx_t
ush_comm_realm_sigreg_connidx_of(const ush_comm_realm_sigreg_t msg);

const ush_sigid_t *
ush_comm_realm_sigreg_sigid_of(const ush_comm_realm_sigreg_t msg);

ush_pvoid_t
ush_comm_realm_sigreg_cb_done_of(const ush_comm_realm_sigreg_t msg);

const ush_pvoid_t *
ush_comm_realm_sigreg_cb_rcv_of(const ush_comm_realm_sigreg_t msg);

ush_pipe_t
ush_comm_realm_sigreg_pipe_of(const ush_comm_realm_sigreg_t msg);

ush_size_t
ush_comm_realm_sigreg_sizeof(void);

ush_ret_t
ush_comm_realm_sigreg_destroy(ush_comm_realm_sigreg_t *msg);

void
ush_comm_realm_sigreg_testpoint(const ush_comm_realm_sigreg_t msg);

#endif // USH_COMM_REALM_SIGREG_H
