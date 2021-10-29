#ifndef USH_SYNC_H
#define USH_SYNC_H

#include "time.h"

#include "ush_type_pub.h"
#include "ush_connect.h"

typedef struct ush_connect_sync_s * ush_connect_sync_t;

ush_ret_t ush_connect_sync_create(ush_connect_sync_t *pSync,
                                    ush_connect_t     conn);

ush_ret_t ush_connect_sync_destroy(ush_connect_sync_t *pSync);

ush_ret_t ush_connect_sync_lock(ush_connect_sync_t sync);

ush_ret_t ush_connect_sync_unlock(ush_connect_sync_t sync);

ush_ret_t ush_connect_sync_wait(ush_connect_sync_t sync,
                                const struct timespec *pDL);

ush_ret_t ush_connect_sync_signal(ush_connect_sync_t sync,
                                  ush_connidx_t      idx,
                                  ush_cert_t         cert);


#endif // USH_SYNC_H
