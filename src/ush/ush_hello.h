#ifndef USH_HELLO_H
#define USH_HELLO_H

typedef struct hello_msg * ush_hello_msg_t;

#include "ush_comm_touch.h"
#include "ush_sync.h"

#define USH_HELLO_NAME_LEN_MAX (32)
#define USH_HELLO_NAME_LEN_MIN (3)


ush_ret_t
ush_hello_create(ush_hello_msg_t      hello,
                const ush_char_t     *pName,
                ush_sync_hello_ack_t  ack);

ush_ret_t ush_hello_destroy(ush_hello_msg_t hello);

size_t ush_hello_msg_size();

#endif // USH_HELLO_H
