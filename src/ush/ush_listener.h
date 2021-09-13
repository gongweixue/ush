#ifndef USH_LISTENER_H
#define USH_LISTENER_H

#include "mqueue.h"

#include "ush_type_pub.h"

typedef struct listener_t*    ush_listener_t;


ush_ret_t ush_listener_alloc(ush_listener_t *listener);

ush_ret_t ush_listener_open(ush_listener_t listener, const ush_char_t *path);

ush_ret_t ush_listener_close(ush_listener_t listener);

ush_ret_t ush_listener_destroy(ush_listener_t listener);


#endif // USH_LISTENER_H
