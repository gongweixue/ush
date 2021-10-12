#ifndef USH_COMM_LSTNR_HOWAREYOU_H
#define USH_COMM_LSTNR_HOWAREYOU_H

#include "ush_comm_lstnr.h"
#include "ush_type_pub.h"

typedef struct ush_hello_ack * ush_sync_hello_ack_t;

typedef struct lstnr_hay * ush_comm_lstnr_hay_t;

ush_comm_lstnr_hay_t
ush_comm_lstnr_hay_create(ush_pvoid_t sync,
                          ush_s32_t   idx,
                          ush_s32_t   cert);

ush_sync_hello_ack_t
ush_comm_lstnr_hay_ack_of(ush_comm_lstnr_hay_t msg);

ush_s32_t
ush_comm_lstnr_hay_remote_idx_of(ush_comm_lstnr_hay_t msg);

ush_s32_t
ush_comm_lstnr_hay_cert_of(ush_comm_lstnr_hay_t msg);

ush_size_t
ush_comm_lstnr_hay_sizeof();

ush_ret_t
ush_comm_lstnr_hay_destroy(ush_comm_lstnr_hay_t *pMsg);

void
ush_comm_lstnr_hay_testpoint(ush_comm_lstnr_hay_t msg);

#endif // USH_COMM_LSTNR_HOWAREYOU_H
