#ifndef USHD_DIST_FIFO_MSG_H
#define USHD_DIST_FIFO_MSG_H

#include "ush_type_pub.h"
#include "ush_sigid_pub.h"

#define USHD_DIST_FIFO_MSG_MAX_SIZE  (128)

typedef enum DIST_FIFO_MSG_TYPE {
    USHD_DIST_FIFO_MSG_TYPE_HAY,
    USHD_DIST_FIFO_MSG_TYPE_SIGREG_ACK,
    USHD_DIST_FIFO_MSG_TYPE_SIG_UPD,
    USHD_DIST_FIFO_MSG_TYPE_STOP
} DIST_FIFO_MSG_TYPE;

// base
typedef struct dist_fifo_msg_desc_s {
    DIST_FIFO_MSG_TYPE type;
} dist_fifo_msg_d;

////////////////////////////////////////////////////////////////////////////////

typedef struct dist_fifo_msg_hay_s {
    dist_fifo_msg_d            desc;
    ush_pvoid_t                sync;
    ush_connidx_t              idx;
    ush_cert_t                 cert;
    ush_s64_t                  deadline;
} dist_fifo_msg_hay;

typedef struct dist_fifo_msg_sigreg_ack_s {
    dist_fifo_msg_d            desc;
    ush_sigid_t                sigid[USH_SIGREG_CONF_MAX];
    ush_bool_t                 success[USH_SIGREG_CONF_MAX];
    ush_pipe_t                 pipe;
    ush_pvoid_t                done;
} dist_fifo_msg_sigreg_ack;

typedef struct dist_fifo_msg_sig_upd_s {
    dist_fifo_msg_d            desc;
    ush_sigid_t                sigid;
    ush_sig_val_t              val;
    ush_pvoid_t                rcv;
    ush_u32_t                  ver;
} dist_fifo_msg_sig_upd;

typedef struct dist_fifo_msg_stop_s {
    dist_fifo_msg_d    desc;
} dist_fifo_msg_stop;

#endif // USHD_DIST_FIFO_MSG_H
