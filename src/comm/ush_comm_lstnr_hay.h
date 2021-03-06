#ifndef USH_COMM_LSTNR_HAY_H
#define USH_COMM_LSTNR_HAY_H

#include "ush_type_pub.h"

typedef struct ush_connect_sync_s * ush_connect_sync_t;

typedef struct lstnr_hay_s * ush_comm_lstnr_hay_t;

ush_comm_lstnr_hay_t
ush_comm_lstnr_hay_create(ush_pvoid_t     sync,
                          ush_connidx_t   idx,
                          ush_cert_t      cert,
                          ush_s64_t       deadline);

ush_pvoid_t
ush_comm_lstnr_hay_sync_of(ush_comm_lstnr_hay_t msg);

ush_connidx_t
ush_comm_lstnr_hay_connidx_of(ush_comm_lstnr_hay_t msg);

ush_cert_t
ush_comm_lstnr_hay_cert_of(ush_comm_lstnr_hay_t msg);

ush_bool_t
ush_comm_lstnr_hay_outofdate(ush_comm_lstnr_hay_t msg);

ush_size_t
ush_comm_lstnr_hay_sizeof(void);

ush_ret_t
ush_comm_lstnr_hay_destroy(ush_comm_lstnr_hay_t *pMsg);

void
ush_comm_lstnr_hay_testpoint(const ush_comm_lstnr_hay_t msg);

#endif // USH_COMM_LSTNR_HAY_H
