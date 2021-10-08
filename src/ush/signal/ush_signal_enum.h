#ifndef USH_SIGNAL_ENUM_H
#define USH_SIGNAL_ENUM_H


#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif
#define USH_ADD_SIGNAL(GRP, NAME, TY) GRP##_##NAME##_##TY,

typedef enum {
#include "ush_signal_conf"
    USH_SIGNAL_IDX_MAX
} ush_signal_idx_t;


#ifdef USH_ADD_SIGNAL
#undef USH_ADD_SIGNAL
#endif

#endif // USH_SIGNAL_ENUM_H
