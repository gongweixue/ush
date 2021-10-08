#ifndef USH_SIGNAL_REGISTER_H
#define USH_SIGNAL_REGISTER_H

#include "ush_type_pub.h"

typedef void (*ush_signal_callback_t)(void *ptr);

#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif

#define USH_ADD_SIGNAL(GRP, NAME, TY)                   \
ush_ret_t ush_register_cb_of_##GRP##_##NAME(ush_signal_callback_t cb);

#include "ush_signal_conf"

#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif

#endif // USH_SIGNAL_REGISTER_H
