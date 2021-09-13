#ifndef USH_HELLO_H
#define USH_HELLO_H

#include "ush_comm_touch.h"
#include "ush_sync.h"

#define USH_HELLO_NAME_LEN_MAX (32)
#define USH_HELLO_NAME_LEN_MIN (3)

typedef struct ush_hello_msg_t {
    touch_msg_desc_t desc;
    ush_s8_t         name[USH_HELLO_NAME_LEN_MAX];
    void            *ackSync;
    ush_s32_t        cert;
} ush_hello_msg_t;

ush_ret_t
ush_hello_composite(ush_hello_msg_t      *pHello,
                    const char           *pName,
                    ush_sync_hello_ack_t     *pAckHdl);

#endif // USH_HELLO_H
