
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_sig_pub.h"

#include "tch/sig/ush_comm_tch_sig_set.h"

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

typedef void (*ush_sig_set_func_t)(ush_sig_val_t *, ush_pvoid_t);

#ifdef USH_ADD_SIG_ID
#undef USH_ADD_SIG_ID
#endif
#define USH_ADD_SIG_ID(GRP, NAME, TY) ush_sig_id_set_##TY,

static ush_sig_set_func_t set_functions[] = {
    NULL,
#include "ush_sig_conf"
    NULL
};


ush_ret_t
ush_sig_set(ush_pipe_t pipe, ush_sig_id_t sigid, const ush_pvoid_t pval) {
    if (USH_INVALID_PIPE == pipe || !ush_sig_id_valid(sigid) || !pval) {
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

    ush_tch_t touch = NULL;
    if (USH_RET_OK != ush_connect_get_tch(conn, &touch)) {
        ush_log(LOG_LVL_ERROR, "conn's touch getting failed");
        return USH_RET_FAILED;
    }

    ush_sig_val_t val;

    set_functions[sigid](&val, pval);

    ush_comm_tch_sig_set_t msg = NULL;
    ush_ret_t ret =
        ush_comm_tch_sig_set_create(&msg, idx, cert, sigid, val, pipe);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sig_set_msg create failed");
        return ret;
    }

    ret = ush_tch_send(touch, (const ush_char_t*)msg,
                       ush_comm_tch_sig_set_sizeof(),
                       USH_COMM_TCH_SEND_PRIO_SIG_SET);

    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sent sigid reg msg failed");
    }
    ush_comm_tch_sig_set_destroy(&msg); // destroy in any case

    return ret;
}
