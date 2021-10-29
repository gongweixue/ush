#ifndef USH_COMM_TCH_HELLO_H
#define USH_COMM_TCH_HELLO_H

#include "ush_type_pub.h"

typedef struct ush_hello_ack_s * ush_sync_hello_ack_t;

typedef struct tch_hello_s * ush_comm_tch_hello_t;

ush_ret_t
ush_comm_tch_hello_create(ush_comm_tch_hello_t    *pHello,
                          const ush_char_t        *name,
                          ush_sync_hello_ack_t    *pAck,
                          ush_cert_t               cert);

ush_ret_t
ush_comm_tch_hello_destroy(ush_comm_tch_hello_t *pHello);

size_t
ush_comm_tch_hello_sizeof(void);

const ush_char_t *
ush_comm_tch_hello_name_of(const ush_comm_tch_hello_t msg);

ush_pvoid_t
ush_comm_tch_hello_ack_of(const ush_comm_tch_hello_t msg);

ush_cert_t
ush_comm_tch_hello_cert_of(const ush_comm_tch_hello_t msg);

void
ush_comm_tch_hello_testpoint(const ush_comm_tch_hello_t msg);

#endif // USH_COMM_TCH_HELLO_H
