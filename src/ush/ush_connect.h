#ifndef USH_CONNECT_H
#define USH_CONNECT_H

#include "ush_type_pub.h"
#include "ush_comm_desc.h"

typedef struct ush_connect * ush_connect_t;

ush_ret_t ush_connect_create(ush_connect_t *pConn, const ush_char_t *name);

ush_ret_t ush_connect_destroy(ush_connect_t *pConn);

ush_ret_t ush_connect_get_cert(ush_connect_t conn, ush_cert_t *ptr);

ush_ret_t ush_connect_set_connidx(ush_connect_t conn, ush_connidx_t idx);

ush_ret_t ush_connect_get_connidx(const ush_connect_t conn, ush_connidx_t *ptr);

ush_ret_t ush_connect_link(ush_connect_t conn, ush_u16_t timeout);

ush_ret_t ush_connect_send(ush_connect_t conn, const ush_comm_d *msg);

#endif // USH_CONNECT_H
