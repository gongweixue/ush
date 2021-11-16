#ifndef USH_COMM_TCH_GOODBYE_H
#define USH_COMM_TCH_GOODBYE_H

#include "ush_type_pub.h"

typedef struct tch_goodbye_s * ush_comm_tch_goodbye_t;

ush_ret_t
ush_comm_tch_goodbye_create(ush_comm_tch_goodbye_t  *pGoodbye,
                            ush_connidx_t            connidx,
                            ush_cert_t               cert);

ush_ret_t
ush_comm_tch_goodbye_destroy(ush_comm_tch_goodbye_t *pGoodbye);

size_t
ush_comm_tch_goodbye_sizeof(void);

ush_connidx_t
ush_comm_tch_goodbye_connidx_of(const ush_comm_tch_goodbye_t msg);

ush_cert_t
ush_comm_tch_goodbye_cert_of(const ush_comm_tch_goodbye_t msg);

void
ush_comm_tch_goodbye_testpoint(const ush_comm_tch_goodbye_t msg);

#endif // USH_COMM_TCH_GOODBYE_H
