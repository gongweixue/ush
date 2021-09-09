#ifndef USH_CONNECTION_H
#define USH_CONNECTION_H

typedef struct touch_handle_t touch_handle_t;

typedef struct listener_handle_t listener_handle_t;

typedef struct connection_ident connection_ident;

typedef struct connection_t connection_t;

ush_bool_t connection_ident_valid(const connection_t *pConn);
ush_ret_t connection_alloc(connection_t *pConn);

#endif // USH_CONNECTION_H
