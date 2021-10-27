
#include "stdlib.h"
#include "string.h"

#include "ush_log.h"
#include "realm/ush_comm_realm.h"
#include "realm/sig/ush_comm_realm_sig.h"
#include "realm/sig/ush_comm_realm_sigset.h"


typedef struct comm_realm_sigset {
    ush_comm_realm_sig_d         desc;
    ush_connidx_t                connidx;
    ush_cert_t                   cert;
    ush_sig_id_t                 sigid;
    ush_sig_val_t                val;
    ush_pipe_t                   pipe;
} USH_COMM_MSG_PACKED  * ush_comm_realm_sigset_t;


ush_ret_t
ush_comm_realm_sigset_create(ush_comm_realm_sigset_t  *pMsg,
                             ush_connidx_t             connidx,
                             ush_cert_t                cert,
                             ush_sig_id_t              sigid,
                             ush_sig_val_t             val,
                             ush_pipe_t                pipe)
{
    if (!pMsg) {
        ush_log(LOG_LVL_ERROR, "ptr NULL");
        return USH_RET_WRONG_PARAM;
    }

    if (USHD_INVALID_CONN_IDX_VALUE == connidx ||
        USH_CONN_IDX_MAX <= connidx            ||
        USH_INVALID_CERT_VALUE == cert         ||
        !ush_sig_id_check(sigid)               ||
        USH_INVALID_PIPE == pipe) {
        ush_log(LOG_LVL_ERROR, "wrong parameters");
        *pMsg = NULL;
        return USH_RET_WRONG_PARAM;
    }

    ush_comm_realm_sigset_t tmp =
        (ush_comm_realm_sigset_t)malloc(sizeof(struct comm_realm_sigset));
    if (!tmp) {
        ush_log(LOG_LVL_ERROR, "out of mem");
        *pMsg = NULL;
        return USH_RET_OUT_OF_MEM;
    }

    tmp->desc.desc.desc.port = USH_COMM_PORT_REALM;
    tmp->desc.desc.catalog   = USH_COMM_REALM_MSG_CATALOG_SIG;
    tmp->desc.intent         = USH_COMM_REALM_SIG_INTENT_SET;
    tmp->connidx             = connidx;
    tmp->cert                = cert;
    tmp->sigid               = sigid;
    tmp->val.dataMAX         = val.dataMAX;
    tmp->pipe                = pipe;

    *pMsg = tmp;

    return USH_RET_OK;
}

ush_s32_t
ush_comm_realm_sigset_get_connidx(const ush_comm_realm_sigset_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return 0;
    }
    return msg->connidx;
}

ush_cert_t
ush_comm_realm_sigset_get_cert(const ush_comm_realm_sigset_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return USH_INVALID_CERT_VALUE;
    }
    return msg->cert;
}

ush_sig_id_t
ush_comm_realm_sigset_get_sigid(const ush_comm_realm_sigset_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return USH_SIG_ID_INVALID;
    }
    return msg->sigid;
}

ush_sig_val_t
ush_comm_realm_sigset_get_val(const ush_comm_realm_sigset_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return (ush_sig_val_t)(0);
    }
    return msg->val;
}

ush_pipe_t
ush_comm_realm_sigset_get_pipe(const ush_comm_realm_sigset_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: null")
        return USH_INVALID_PIPE;
    }

    return msg->pipe;
}

ush_ret_t
ush_comm_realm_sigset_destroy(ush_comm_realm_sigset_t *pMsg) {
    if (!pMsg) {
        return USH_RET_OK;
    }

    ush_comm_realm_sigset_t msg = *pMsg;
    if (!msg) {
        return USH_RET_OK;
    }

    free(msg);
    *pMsg = NULL;

    return USH_RET_OK;
}

ush_size_t
ush_comm_realm_sigset_sizeof(void) {
    return sizeof(struct comm_realm_sigset);
}
