
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_sig_pub.h"

#include "realm/sig/ush_comm_realm_sigset.h"

// define set function prototype
#define DEFINE_SIG_ID_SET_FUNC(TY)                                             \
static inline void ush_sig_id_set_##TY(ush_sig_val_t *pDst,                    \
                                       const ush_pvoid_t pSrc) {               \
    pDst->data##TY = *(TY*)pSrc;                                               \
}

DEFINE_SIG_ID_SET_FUNC(BOOL)
DEFINE_SIG_ID_SET_FUNC(CHAR)
DEFINE_SIG_ID_SET_FUNC(U8)
DEFINE_SIG_ID_SET_FUNC(S8)
DEFINE_SIG_ID_SET_FUNC(U16)
DEFINE_SIG_ID_SET_FUNC(S16)
DEFINE_SIG_ID_SET_FUNC(U32)
DEFINE_SIG_ID_SET_FUNC(S32)
DEFINE_SIG_ID_SET_FUNC(U64)
DEFINE_SIG_ID_SET_FUNC(S64)
DEFINE_SIG_ID_SET_FUNC(FP32)
DEFINE_SIG_ID_SET_FUNC(FP64)

typedef void (*ush_sigset_func_t)(ush_sig_val_t *, ush_pvoid_t);

#ifdef USH_ADD_SIG_ID
#undef USH_ADD_SIG_ID
#endif
#define USH_ADD_SIG_ID(GRP, NAME, TY) ush_sig_id_set_##TY,

static ush_sigset_func_t set_functions[] = {
    NULL, // for USH_SIG_ID_INVALID,
#include "ush_sig_conf"
    NULL // for USH_SIG_ID_MAX
};


ush_ret_t
ush_sigset(ush_pipe_t pipe, ush_sig_id_t sigid, const ush_pvoid_t pval) {
    if (USH_INVALID_PIPE == pipe || !ush_sig_id_check(sigid) || !pval) {
        ush_log(LOG_LVL_ERROR, "parameters not correct!");
        return USH_RET_WRONG_PARAM;
    }

    ush_connect_t conn = (ush_connect_t)pipe;
    ush_cert_t cert = USH_INVALID_CERT_VALUE;
    if (USH_RET_OK != ush_connect_get_cert(conn, &cert)) {
        ush_log(LOG_LVL_ERROR, "invalid cert");
        return USH_RET_WRONG_SEQ;
    }

    ush_connidx_t idx = 0;
    if (USH_RET_OK != ush_connect_get_connidx(conn, &idx)) {
        ush_log(LOG_LVL_ERROR, "remote conn idx getting failed");
        return USH_RET_WRONG_SEQ;
    }

    ush_sig_val_t val;

    set_functions[sigid](&val, pval);

    ush_comm_realm_sigset_t msg = NULL;
    ush_ret_t ret =
        ush_comm_realm_sigset_create(&msg, idx, cert, sigid, val, pipe);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sigset_msg create failed");
        return ret;
    }

    ret = ush_connect_send(conn, (const ush_comm_d *)msg);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sent sigid reg msg failed");
    }
    ush_comm_realm_sigset_destroy(&msg); // destroy in any case

    return ret;
}