#ifndef USH_CONNECT_H
#define USH_CONNECT_H

#include "ush_type_pub.h"
#include "ush_pipe_pub.h"
#include "ush_pipe_touch.h"

typedef struct listener_handle_t listener_handle_t;

typedef ush_u64_t connect_ident;
#define CONNECT_IDENT_VALUE_DEFAULT (0xFFFFFFFFFFFFFFFF)

typedef struct connect_t connect_t;

ush_bool_t connect_ident_valid(const connect_t *pConn);

ush_ret_t connect_alloc(connect_t *pConn);

ush_ret_t connect_init(connect_t *pConn);

ush_ret_t connect_destroy(connect_t *pConn);

connect_ident connect_make_ident(ush_u32_t idx, ush_u32_t certify);
ush_ret_t connect_set_ident(connect_t *pConn, connect_ident ident);

ush_ret_t connect_get_ident(connect_t *pConn, connect_ident *pIdent);

ush_ret_t connect_set_ident(connect_t *pConn, connect_ident ident);

ush_ret_t connect_get_ident(connect_t *pConn, connect_ident *pIdent);

// ush_ret_t connect_set_state(connect_t *pConn, ush_pp_state_t state);

// ush_ret_t connect_get_state(connect_t *pConn, ush_pp_state_t *pState);

ush_ret_t connect_get_touch(connect_t *pConn, ush_pipe_touch_t *ptr);

ush_ret_t connect_open_touch(connect_t *pConn);

ush_ret_t connect_close_touch(connect_t *pConn);

#endif // USH_CONNECT_H
