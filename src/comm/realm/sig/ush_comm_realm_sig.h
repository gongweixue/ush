#ifndef USH_COMM_REALM_SIG_H
#define USH_COMM_REALM_SIG_H

#include "ush_type_pub.h"

#include "ush_sig_pub.h"
#include "ush_comm_desc.h"

typedef enum USH_COMM_REALM_SIG_INTENT {
    USH_COMM_REALM_SIG_INTENT_REG,
    USH_COMM_REALM_SIG_INTENT_SET,
    USH_COMM_REALM_SIG_INTENT_TEASE
} USH_COMM_REALM_SIG_INTENT;


typedef struct ush_comm_realm_sig_desc_s {
    ush_comm_realm_msg_d        desc;
    USH_COMM_REALM_SIG_INTENT   intent;
    ush_connidx_t               connidx;
    ush_cert_t                  cert;
    ush_sigid_t                 sigid;
} USH_COMM_MSG_PACKED ush_comm_realm_sig_d;


#endif // USH_COMM_REALM_SIG_H
