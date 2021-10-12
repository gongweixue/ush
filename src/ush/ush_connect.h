#ifndef USH_CONNECT_H
#define USH_CONNECT_H

typedef struct ush_connect * ush_connect_t;

#include "listener/ush_listener.h"
#include "ush_touch.h"
#include "ush_type_pub.h"

ush_ret_t ush_connect_create(ush_connect_t *pConn, const ush_char_t *name);

ush_ret_t ush_connect_destroy(ush_connect_t *pConn);

ush_ret_t ush_connect_get_cert(ush_connect_t conn, ush_s32_t *ptr);

ush_ret_t ush_connect_set_remote_idx(ush_connect_t conn, ush_s32_t idx);

ush_ret_t ush_connect_get_remote_idx(const ush_connect_t conn, ush_s32_t *ptr);

ush_ret_t ush_connect_get_touch(ush_connect_t conn, ush_touch_t *ptr);

ush_ret_t ush_connect_get_listener(ush_connect_t conn, ush_listener_t *ptr);

ush_ret_t ush_connect_cs_enter(ush_connect_t conn);

ush_ret_t ush_connect_cs_exit(ush_connect_t conn);

// ush_ret_t ush_connect_listen_start(ush_connect_t conn, const ush_char_t *path);


#endif // USH_CONNECT_H
