
#include "stdlib.h"

#include "ush_log.h"
#include "ush_comm_realm.h"
#include "ush_comm_realm_sig.h"
#include "ush_comm_realm_sigreg.h"


typedef struct comm_realm_sigreg_s {
    ush_comm_realm_sig_d         desc;
    ush_pvoid_t                  done;
    ush_pvoid_t                  rcv;
    ush_pipe_t                   pipe;
} USH_COMM_MSG_PACKED  * ush_comm_realm_sigreg_t;


ush_ret_t
ush_comm_realm_sigreg_create(ush_comm_realm_sigreg_t  *pMsg,
                             ush_connidx_t             connidx,
                             ush_cert_t                cert,
                             ush_sigid_t               sigid,
                             const ush_pvoid_t         done,
                             const ush_pvoid_t         rcv,
                             ush_pipe_t                pipe)
{
    if (!pMsg) {
        ush_log(LOG_LVL_ERROR, "ptr NULL");
        return USH_RET_WRONG_PARAM;
    }

    ush_comm_realm_sigreg_t tmp =
        (ush_comm_realm_sigreg_t)malloc(sizeof(struct comm_realm_sigreg_s));
    if (!tmp) {
        ush_log(LOG_LVL_ERROR, "out of mem");
        return USH_RET_OUT_OF_MEM;
    }

    tmp->desc.desc.desc.port = USH_COMM_PORT_REALM;
    tmp->desc.desc.catalog   = USH_COMM_REALM_MSG_CATALOG_SIG;
    tmp->desc.intent         = USH_COMM_REALM_SIG_INTENT_REG;
    tmp->desc.connidx        = connidx;
    tmp->desc.cert           = cert;
    tmp->desc.sigid          = sigid;
    tmp->done                = done;
    tmp->rcv                 = rcv;
    tmp->pipe                = pipe;

    *pMsg = tmp;

    return USH_RET_OK;
}


ush_cert_t
ush_comm_realm_sigreg_cert_of(const ush_comm_realm_sigreg_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return USH_INVALID_CERT_VALUE;
    }
    return msg->desc.cert;
}

ush_s32_t
ush_comm_realm_sigreg_connidx_of(const ush_comm_realm_sigreg_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return 0;
    }
    return msg->desc.connidx;
}

ush_sigid_t
ush_comm_realm_sigreg_sigid_of(const ush_comm_realm_sigreg_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return USH_SIG_ID_INVALID;
    }
    return msg->desc.sigid;
}

ush_pvoid_t
ush_comm_realm_sigreg_cb_done_of(const ush_comm_realm_sigreg_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: null")
        return NULL;
    }

    return msg->done;
}

ush_pvoid_t
ush_comm_realm_sigreg_cb_rcv_of(const ush_comm_realm_sigreg_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: null")
        return NULL;
    }

    return msg->rcv;
}

ush_pipe_t
ush_comm_realm_sigreg_pipe_of(const ush_comm_realm_sigreg_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: null")
        return USH_INVALID_PIPE;
    }

    return msg->pipe;
}


ush_ret_t
ush_comm_realm_sigreg_destroy(ush_comm_realm_sigreg_t *pMsg) {
    if (!pMsg) {
        ush_log(LOG_LVL_INFO, "msg ptr null");
        return USH_RET_OK;
    }

    ush_comm_realm_sigreg_t msg = *pMsg;
    if (!msg) {
        ush_log(LOG_LVL_INFO, "msg ptr null");
        return USH_RET_OK;
    }

    free(msg);
    *pMsg = NULL;

    return USH_RET_OK;
}

ush_size_t
ush_comm_realm_sigreg_sizeof(void) {
    return sizeof(struct comm_realm_sigreg_s);
}

void
ush_comm_realm_sigreg_testpoint(const ush_comm_realm_sigreg_t msg) {
    (void)msg;
}