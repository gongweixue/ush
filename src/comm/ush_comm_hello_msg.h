#ifndef USH_COMM_HELLO_MSG_H
#define USH_COMM_HELLO_MSG_H

typedef struct ush_comm_hello_msg * ush_comm_hello_msg_t;

#include "ush_comm_touch.h"

#define USH_COMM_HELLO_MSG_NAME_LEN_MAX (32)
#define USH_COMM_HELLO_MSG_NAME_LEN_MIN (3)
#define USH_COMM_HELLO_MSG_PRIO         (0)

ush_ret_t
ush_comm_hello_msg_create(ush_comm_hello_msg_t      *pHello,
                          const ush_char_t          *pName,
                          ush_vptr_t                 pAck,
                          ush_u32_t                  cert);

ush_ret_t ush_comm_hello_msg_destroy(ush_comm_hello_msg_t *pHello);

size_t ush_comm_hello_msg_size();

const ush_char_t * ush_comm_hello_msg_get_name(const ush_comm_hello_msg_t msg);

const ush_vptr_t ush_comm_hello_msg_get_ack(const ush_comm_hello_msg_t msg);

ush_s32_t ush_comm_hello_msg_get_cert(const ush_comm_hello_msg_t msg);

void ush_comm_hello_msg_testpoint(const ush_comm_hello_msg_t msg);

#endif // USH_COMM_HELLO_MSG_H
