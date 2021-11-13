#ifndef USH_SIGID_PUB_H
#define USH_SIGID_PUB_H

#include "ush_type_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// define the sig id enum
#ifdef USH_ADD_SIG_ID
#undef USH_ADD_SIG_ID
#endif

// test sig-id redeclaration by dropping the type of sigid
#define USH_ADD_SIG_ID(GRP, NAME, TY) REDECLARATION_CHECK_##GRP##_##NAME,
enum USH_REDECLARATION_CHECK {
#include "ush_sig_conf"
};
#undef USH_ADD_SIG_ID // disable the macro

#define USH_ADD_SIG_ID(GRP, NAME, TY) USH_SIG_ID_##GRP##_##NAME##_##TY,
typedef enum {
    USH_SIG_ID_INVALID,
#include "ush_sig_conf"
    USH_SIG_ID_MAX
} ush_sigid_t;

#ifdef USH_ADD_SIG_ID
#undef USH_ADD_SIG_ID
#endif

// sig-id validation
static inline ush_bool_t ush_sigid_check(ush_sigid_t id) {
    ush_bool_t ret = (id > USH_SIG_ID_INVALID && id < USH_SIG_ID_MAX);
    return ret;
}



#ifdef __cplusplus
}
#endif


#endif // USH_SIGID_PUB_H
