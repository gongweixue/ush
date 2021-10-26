#ifndef USH_CONNECT_H
#define USH_CONNECT_H

typedef struct ush_connect * ush_connect_t;

#include "ush_type_pub.h"
#include "lstnr/ush_lstnr.h"
#include "ush_tch.h"

ush_ret_t ush_connect_create(ush_connect_t *pConn, const ush_char_t *shortname_ts);

ush_ret_t ush_connect_destroy(ush_connect_t *pConn);

ush_ret_t ush_connect_get_cert(ush_connect_t conn, ush_cert_t *ptr);

ush_ret_t ush_connect_set_connidx(ush_connect_t conn, ush_connidx_t idx);

ush_ret_t ush_connect_get_connidx(const ush_connect_t conn, ush_connidx_t *ptr);

ush_ret_t ush_connect_get_tch(ush_connect_t conn, ush_tch_t *ptr);

ush_ret_t ush_connect_get_lstnr(ush_connect_t conn, ush_lstnr_t *ptr);

// ush_ret_t ush_connect_listen_start(ush_connect_t conn, const ush_char_t *path);


#endif // USH_CONNECT_H
