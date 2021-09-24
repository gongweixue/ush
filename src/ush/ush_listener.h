#ifndef USH_LISTENER_H
#define USH_LISTENER_H


typedef struct ush_listener * ush_listener_t;

#include "mqueue.h"

#include "ush_type_pub.h"


ush_ret_t ush_listener_open_and_start(ush_listener_t *Listener);

ush_ret_t ush_listener_stop_and_close(ush_listener_t *pListener);


#endif // USH_LISTENER_H
