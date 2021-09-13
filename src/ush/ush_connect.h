#ifndef USH_CONNECT_H
#define USH_CONNECT_H

typedef struct connect*    ush_connect_t;

#include "ush_listener.h"
#include "ush_touch.h"
#include "ush_type_pub.h"

typedef ush_u64_t ush_connect_ident;
#define CONNECT_IDENT_VALUE_DEFAULT (0xFFFFFFFFFFFFFFFF)


ush_ret_t ush_connect_alloc(ush_connect_t *pConn);

ush_ret_t ush_connect_init(ush_connect_t conn);

ush_ret_t ush_connect_destroy(ush_connect_t conn);


ush_connect_ident ush_connect_make_ident(ush_u32_t idx, ush_u32_t certify);

ush_bool_t ush_connect_ident_valid(const ush_connect_t conn);

ush_ret_t ush_connect_set_ident(ush_connect_t conn, ush_connect_ident ident);

ush_ret_t ush_connect_get_ident(ush_connect_t conn, ush_connect_ident *pIdent);


ush_ret_t ush_connect_get_touch(ush_connect_t conn, ush_touch_t *ptr);

// ush_ret_t ush_connect_open_touch(ush_connect_t *pConn);

// ush_ret_t ush_connect_close_touch(ush_connect_t *pConn);


ush_ret_t ush_connect_get_listener(ush_connect_t conn, ush_listener_t *ptr);

ush_ret_t ush_connect_open_listener(ush_connect_t conn);

ush_ret_t ush_connect_close_listener(ush_connect_t conn);


#endif // USH_CONNECT_H
