#ifndef USH_SYNC_H
#define USH_SYNC_H

#include "time.h"

#include "ush_type_pub.h"
#include "ush_connect.h"


typedef struct ush_hello_ack * ush_sync_hello_ack_t;

ush_ret_t ush_sync_hello_ack_create(ush_sync_hello_ack_t *pAck,
                                    ush_connect_t         conn);

ush_ret_t ush_sync_hello_ack_wait(ush_sync_hello_ack_t   ack,
                                  const struct timespec *pDL);

ush_ret_t ush_sync_hello_ack_signal_and_destroy(ush_sync_hello_ack_t *pAck,
                                                ush_connidx_t         idx,
                                                ush_cert_t            cert);


#endif // USH_SYNC_H
