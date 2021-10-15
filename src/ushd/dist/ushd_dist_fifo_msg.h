#ifndef USHD_DIST_FIFO_MSG_H
#define USHD_DIST_FIFO_MSG_H

#include "ush_type_pub.h"
#include "ush_sig_enum.h"

#define USHD_DIST_FIFO_MSG_MAX_SIZE  (128)

typedef enum DIST_FIFO_MSG_TYPE {
    USHD_DIST_FIFO_MSG_TYPE_HAY,
    USHD_DIST_FIFO_MSG_TYPE_SIG_REG_ACK
} DIST_FIFO_MSG_TYPE;

// base
typedef struct dist_fifo_msg_desc {
    DIST_FIFO_MSG_TYPE type;
} dist_fifo_msg_d;

////////////////////////////////////////////////////////////////////////////////

typedef struct dist_fifo_msg_hay {
    dist_fifo_msg_d            desc;
    ush_pvoid_t                ack_sync;
    ush_s32_t                  idx;
    ush_s32_t                  cert;
} dist_fifo_msg_hay;

typedef struct dist_fifo_msg_sig_reg_ack {
    dist_fifo_msg_d            desc;
    ush_bool_t                 success;
    ush_sig_id_t               sigid;
    ush_pipe_t                 pipe;
    ush_pvoid_t                done;
} dist_fifo_msg_sig_reg_ack;

#endif // USHD_DIST_FIFO_MSG_H
