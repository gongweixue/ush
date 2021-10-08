
#include "ush_signal_register.h"

#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif
#define USH_ADD_SIGNAL(GRP, NAME, TY)                                   \
ush_ret_t ush_register_cb_of_##GRP##_##NAME(ush_signal_callback_t cb) { \
    return USH_RET_OK;                                                  \
}

#include "ush_signal_conf"

#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif
