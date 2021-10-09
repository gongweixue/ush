#ifndef USH_COMM_HELLO_MSG_H
#define USH_COMM_HELLO_MSG_H

#include "ush_comm_touch.h"

typedef struct ush_hello_ack * ush_sync_hello_ack_t;

typedef struct hello_msg * ush_comm_hello_msg_t;

#define USH_COMM_HELLO_MSG_PRIO         (0)

ush_ret_t
ush_comm_hello_msg_create(ush_comm_hello_msg_t      *pHello,
                          const ush_char_t          *name,
                          ush_sync_hello_ack_t       ack,
                          ush_s32_t                  cert);

ush_ret_t         ush_comm_hello_msg_destroy(ush_comm_hello_msg_t *pHello);

size_t            ush_comm_hello_msg_sizeof();
const ush_char_t *ush_comm_hello_msg_name_of(const ush_comm_hello_msg_t msg);
const ush_pvoid_t ush_comm_hello_msg_ack_of(const ush_comm_hello_msg_t msg);
ush_s32_t         ush_comm_hello_msg_cert_of(const ush_comm_hello_msg_t msg);

void ush_comm_hello_msg_testpoint(const ush_comm_hello_msg_t msg);

#endif // USH_COMM_HELLO_MSG_H
