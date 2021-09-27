#ifndef USH_SYNC_H
#define USH_SYNC_H

typedef struct ush_hello_ack * ush_sync_hello_ack_t;

#include "time.h"

#include "ush_connect.h"


typedef ush_ret_t (*ush_sync_hello_ack_wait_cb_t)(ush_sync_hello_ack_t ack);

ush_ret_t ush_sync_hello_ack_create(ush_sync_hello_ack_t *pAck,
                                    ush_connect_t         conn);

ush_ret_t ush_sync_hello_ack_wait(ush_sync_hello_ack_t         ack,
                                  const struct timespec       *pDL,
                                  ush_sync_hello_ack_wait_cb_t cb);

ush_ret_t ush_sync_hello_ack_signal(ush_sync_hello_ack_t ack,
                                    ush_connect_ident ident);

ush_ret_t ush_sync_hello_ack_destroy(ush_sync_hello_ack_t *pAck);



#endif // USH_SYNC_H
