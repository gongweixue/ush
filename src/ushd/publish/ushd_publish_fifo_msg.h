#ifndef USHD_PUBLISH_FIFO_MSG_H
#define USHD_PUBLISH_FIFO_MSG_H

#include "ush_type_pub.h"

#define USHD_PUBLISH_FIFO_MSG_MAX_SIZE  (128)

typedef enum PUBLISH_FIFO_MSG_TYPE {
    USHD_PUBLISH_FIFO_CMD_HOWAREYOU
} PUBLISH_FIFO_MSG_TYPE;

// base
typedef struct publish_fifo_msg_desc {
    PUBLISH_FIFO_MSG_TYPE type;
} publish_fifo_msg_desc;

////////////////////////////////////////////////////////////////////////////////

typedef struct publish_fifo_msg_howareyou {
    publish_fifo_msg_desc      desc;
    ush_vptr_t                 ack_sync;
    ush_u64_t                  ident;
} publish_fifo_msg_howareyou;

#endif // USHD_PUBLISH_FIFO_MSG_H
