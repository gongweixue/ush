#ifndef USH_SIGNAL_ACCESS_H
#define USH_SIGNAL_ACCESS_H

#include "ush_type_pub.h"

#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif
#define USH_ADD_SIGNAL(GRP, NAME, TY)                  \
TY ush_get_data_from_##GRP##_##NAME(void *ptr);

#include "ush_signal_conf"

#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif

#endif // USH_SIGNAL_ACCESS_H
