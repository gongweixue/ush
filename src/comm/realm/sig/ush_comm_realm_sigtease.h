#ifndef USH_COMM_REALM_SIGTEASE_H
#define USH_COMM_REALM_SIGTEASE_H

#include "ush_type_pub.h"
#include "ush_sig_id.h"

typedef struct comm_realm_sigtease_s * ush_comm_realm_sigtease_t;

ush_ret_t
ush_comm_realm_sigtease_create(ush_comm_realm_sigtease_t  *pMsg,
                               ush_connidx_t               connidx,
                               ush_cert_t                  cert,
                               ush_sig_id_t                sigid);

ush_connidx_t
ush_comm_realm_sigtease_connidx_of(const ush_comm_realm_sigtease_t msg);

ush_cert_t
ush_comm_realm_sigtease_cert_of(const ush_comm_realm_sigtease_t msg);

ush_sig_id_t
ush_comm_realm_sigtease_sigid_of(const ush_comm_realm_sigtease_t msg);

ush_size_t
ush_comm_realm_sigtease_sizeof(void);

ush_ret_t
ush_comm_realm_sigtease_destroy(ush_comm_realm_sigtease_t *msg);

#endif // USH_COMM_REALM_SIGTEASE_H
