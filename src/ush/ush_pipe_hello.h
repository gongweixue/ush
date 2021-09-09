#ifndef USH_PIPE_HELLO_H
#define USH_PIPE_HELLO_H

#include "ush_comm_touch.h"
#include "ush_pipe_sync.h"

#define USH_COMM_TOUCH_Q_HELLO_NAME_LEN_MAX (32)
#define USH_COMM_TOUCH_Q_HELLO_NAME_LEN_MIN (3)

typedef struct ush_pipe_msg_hello_t {
    touch_msg_desc_t desc;
    ush_s8_t         name[USH_COMM_TOUCH_Q_HELLO_NAME_LEN_MAX];
    void            *ackSync;
    ush_s32_t        cert;
} ush_pipe_msg_hello_t;

ush_ret_t
ush_pipe_hello_load(ush_pipe_msg_hello_t *pHello,
                    const char*           pName,
                    sync_hello_ack_t     *pAckHdl);

#endif // USH_PIPE_HELLO_H
