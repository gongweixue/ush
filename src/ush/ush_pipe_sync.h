#ifndef USH_PIPE_SYNC_H
#define USH_PIPE_SYNC_H

#include "time.h"

typedef struct ush_pipe_sync_hello_ack_t sync_hello_ack_t;

ush_ret_t ush_sync_hello_ack_create(sync_hello_ack_t *pAck);
ush_ret_t ush_sync_hello_ack_wait(sync_hello_ack_t *pAck, const struct timespec *pDL);
ush_ret_t ush_sync_hello_ack_destroy(sync_hello_ack_t *pAck);

#endif // USH_PIPE_SYNC_H
