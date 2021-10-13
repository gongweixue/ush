#ifndef USH_COMM_TCH_SIG_H
#define USH_COMM_TCH_SIG_H

#include "ush_type_pub.h"

#include "ush_sig_pub.h"
#include "ush_comm_port.h"

typedef enum USH_COMM_TCH_SIG_INTENT {
    USH_COMM_TCH_SIG_INTENT_REG,
    USH_COMM_TCH_SIG_INTENT_MAX
} USH_COMM_TCH_SIG_INTENT;


typedef struct ush_comm_tch_sig_d {
    ush_comm_tch_msg_d        desc;
    USH_COMM_TCH_SIG_INTENT   intent;
} USH_COMM_MSG_PACKED ush_comm_tch_sig_d;


#endif // USH_COMM_TCH_SIG_H
