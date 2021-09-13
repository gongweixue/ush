#ifndef USH_SYNC_H
#define USH_SYNC_H

#include "time.h"

typedef struct ush_sync_hello_ack_t ush_sync_hello_ack_t;

ush_ret_t ush_sync_hello_ack_create(ush_sync_hello_ack_t *pAck);

ush_ret_t ush_sync_hello_ack_wait(ush_sync_hello_ack_t *pAck,
                                  const struct timespec *pDL);

ush_ret_t ush_sync_hello_ack_destroy(ush_sync_hello_ack_t *pAck);

#endif // USH_SYNC_H
