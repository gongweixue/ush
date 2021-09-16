#ifndef USH_COMM_HELLO_MSG_H
#define USH_COMM_HELLO_MSG_H

typedef struct ush_comm_hello_msg * ush_comm_hello_msg_t;

#include "ush_comm_touch.h"

#define USH_COMM_HELLO_MSG_NAME_LEN_MAX (32)
#define USH_COMM_HELLO_MSG_NAME_LEN_MIN (3)


ush_ret_t
ush_comm_hello_msg_create(ush_comm_hello_msg_t      *pHello,
                          const ush_char_t          *pName,
                          void                      *pAck,
                          ush_u32_t                  cert);

ush_ret_t ush_comm_hello_msg_destroy(ush_comm_hello_msg_t *pHello);

size_t ush_comm_hello_msg_size();

#endif // USH_COMM_HELLO_MSG_H
