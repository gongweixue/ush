#ifndef USH_COMM_HOWAREYOU_MSG_H
#define USH_COMM_HOWAREYOU_MSG_H

#include "ush_type_pub.h"
#include "ush_comm_listener.h"

#define USH_COMM_HOWAREYOU_MSG_PRIO         (0)

typedef struct howareyou_msg * ush_comm_howareyou_msg_t;

ush_comm_howareyou_msg_t
ush_comm_howareyou_msg_create(ush_vptr_t sync, ush_s32_t idx, ush_s32_t cert);

ush_vptr_t ush_comm_howareyou_msg_ack_of(ush_comm_howareyou_msg_t msg);
ush_s32_t  ush_comm_howareyou_msg_remote_idx_of(ush_comm_howareyou_msg_t msg);
ush_s32_t  ush_comm_howareyou_msg_cert_of(ush_comm_howareyou_msg_t msg);
ush_size_t ush_comm_howareyou_msg_size_of();

ush_ret_t  ush_comm_howareyou_msg_destroy(ush_comm_howareyou_msg_t *pMsg);

void ush_comm_howareyou_msg_testpoint(ush_comm_howareyou_msg_t msg);

#endif // USH_COMM_HOWAREYOU_MSG_H
