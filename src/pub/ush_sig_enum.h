#ifndef USH_SIG_ENUM_H
#define USH_SIG_ENUM_H


#ifdef USH_ADD_SIG
#undef USH_ADD_SIG
#endif
#define USH_ADD_SIG(GRP, NAME, TY) GRP##_##NAME##_##TY,


#include "ush_type_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
#include "ush_sig_conf"
    USH_SIG_ID_MAX
} ush_sig_id_t;

static inline ush_bool_t ush_sig_id_valid(ush_sig_id_t id) {
    return (id >= 0 && id < USH_SIG_ID_MAX);
}


#ifdef __cplusplus
}
#endif


#ifdef USH_ADD_SIG
#undef USH_ADD_SIG
#endif

#endif // USH_SIG_ENUM_H
