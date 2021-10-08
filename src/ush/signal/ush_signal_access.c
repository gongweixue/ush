
#include "ush_signal_access.h"

#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif
#define USH_ADD_SIGNAL(GRP, NAME, TY)                      \
TY ush_get_data_from_##GRP##_##NAME(void *ptr) {           \
    return *((TY*)(ptr));                                  \
}

#include "ush_signal_conf"

#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif
