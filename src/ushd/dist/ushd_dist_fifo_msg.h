#ifndef USHD_DIST_FIFO_MSG_H
#define USHD_DIST_FIFO_MSG_H

#include "ush_type_pub.h"

#define USHD_DIST_FIFO_MSG_MAX_SIZE  (128)

typedef enum DIST_FIFO_MSG_TYPE {
    USHD_DIST_FIFO_CMD_HOWAREYOU
} DIST_FIFO_MSG_TYPE;

// base
typedef struct dist_fifo_msg_desc {
    DIST_FIFO_MSG_TYPE type;
} dist_fifo_msg_desc;

////////////////////////////////////////////////////////////////////////////////

typedef struct dist_fifo_msg_howareyou {
    dist_fifo_msg_desc         desc;
    ush_vptr_t                 ack_sync;
    ush_s32_t                  idx;
    ush_s32_t                  cert;
} dist_fifo_msg_howareyou;

#endif // USHD_DIST_FIFO_MSG_H
