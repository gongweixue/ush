#ifndef USH_COMM_SIG_MSG_H
#define USH_COMM_SIG_MSG_H

#include "ush_comm_touch.h"

#include "ush_sig_pub.h"

typedef enum USH_COMM_TOUCH_SIG_INTENT {
    USH_COMM_TOUCH_SIG_INTENT_REG,
    USH_COMM_TOUCH_SIG_INTENT_MAX
} USH_COMM_TOUCH_SIG_INTENT;

typedef struct ush_comm_sig_msg_description {
    ush_touch_msg_description desc;
    USH_COMM_TOUCH_SIG_INTENT intent;
} USH_COMM_MSG_PACKED ush_comm_sig_msg_description;

typedef struct ush_comm_sig_msg_reg {
    ush_comm_sig_msg_description desc;
    ush_s32_t                    remote_idx;
    ush_s32_t                    cert;
    ush_sig_id_t                 sig;
    ush_sig_cb_reg_t             done;
    ush_sig_cb_rcv_t             rcv;
    ush_pipe_t                   pipe;
} USH_COMM_MSG_PACKED ush_comm_sig_msg_reg_t;

#endif // USH_COMM_SIG_MSG_H
