#ifndef USH_LISTENER_H
#define USH_LISTENER_H


typedef struct ush_listener * ush_listener_t;

#include "mqueue.h"

#include "ush_type_pub.h"


ush_ret_t ush_listener_alloc(ush_listener_t *Listener);

ush_ret_t ush_listener_open(ush_listener_t listener, const ush_char_t *path);

ush_ret_t ush_listener_close(ush_listener_t listener);

ush_ret_t ush_listener_destroy_with_closing(ush_listener_t *pListener);


#endif // USH_LISTENER_H
