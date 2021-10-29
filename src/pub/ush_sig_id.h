#ifndef USH_SIG_ID_H
#define USH_SIG_ID_H

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
#define USH_ADD_SIG_ID(GRP, NAME, TY) USH_SIG_ID_##GRP##_##NAME,
enum TEST_SIG_REDECLARATION {
#include "ush_sig_conf"
};
#undef USH_ADD_SIG_ID // disable the macro

#define USH_ADD_SIG_ID(GRP, NAME, TY) USH_SIG_ID_##GRP##_##NAME##_##TY,
typedef enum {
    USH_SIG_ID_INVALID,
#include "ush_sig_conf"
    USH_SIG_ID_MAX
} ush_sig_id_t;

#ifdef USH_ADD_SIG_ID
#undef USH_ADD_SIG_ID
#endif

// sig-id validation
static inline ush_bool_t ush_sig_id_check(ush_sig_id_t id) {
    ush_bool_t ret = (id > USH_SIG_ID_INVALID && id < USH_SIG_ID_MAX);
    return ret;
}



#ifdef __cplusplus
}
#endif


#endif // USH_SIG_ID_H
