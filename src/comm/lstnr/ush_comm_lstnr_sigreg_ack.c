#include "stdlib.h"

#include "ush_log.h"
#include "ush_comm_lstnr.h"
#include "ush_comm_lstnr_sigreg_ack.h"


typedef struct lstnr_sigreg_ack {
    ush_comm_lstnr_msg_d         desc;
    ush_bool_t                   success;
    ush_sig_id_t                 sigid;
    ush_pipe_t                   pipe;
    ush_pvoid_t                  done;
} USH_COMM_MSG_PACKED * ush_comm_lstnr_sigreg_ack_t;

ush_comm_lstnr_sigreg_ack_t
ush_comm_lstnr_sigreg_ack_create(ush_bool_t   success,
                                 ush_sig_id_t sigid,
                                 ush_pipe_t   pipe,
                                 ush_pvoid_t  done) {
    ush_comm_lstnr_sigreg_ack_t ret =
        (ush_comm_lstnr_sigreg_ack_t)malloc(sizeof(struct lstnr_sigreg_ack));

    if (!ret) {
        ush_log(LOG_LVL_ERROR, "hay msg mem alloc failed.");
        return NULL;
    }

    ret->desc.desc.port = USH_COMM_PORT_LSTNR;
    ret->desc.catalog   = USH_COMM_LSTNR_MSG_CATALOG_SIGREG_ACK;
    ret->success        = success;
    ret->sigid          = sigid;
    ret->pipe           = pipe;
    ret->done           = done;

    return ret;
}

ush_size_t
ush_comm_lstnr_sigreg_ack_sizeof(void) {
    return sizeof(struct lstnr_sigreg_ack);
}


ush_sig_cb_reg_t
ush_comm_lstnr_sigreg_ack_get_done(const ush_comm_lstnr_sigreg_ack_t msg) {
    if (!msg) {
        return NULL;
    }

    return (ush_sig_cb_reg_t)(msg->done);
}

ush_pipe_t
ush_comm_lstnr_sigreg_ack_get_pipe(const ush_comm_lstnr_sigreg_ack_t msg) {
    if (!msg) {
        return USH_INVALID_PIPE;
    }

    return msg->pipe;
}

ush_sig_id_t
ush_comm_lstnr_sigreg_ack_get_sigid(const ush_comm_lstnr_sigreg_ack_t msg) {
    if (!msg) {
        return USH_SIG_ID_INVALID;
    }

    return msg->sigid;
}

ush_bool_t
ush_comm_lstnr_sigreg_ack_get_success(const ush_comm_lstnr_sigreg_ack_t msg) {
    if (!msg) {
        return 0;
    }

    return msg->success;
}

ush_ret_t
ush_comm_lstnr_sigreg_ack_destroy(ush_comm_lstnr_sigreg_ack_t *pMsg) {
    if (!pMsg) {
        return USH_RET_OK;
    }

    ush_comm_lstnr_sigreg_ack_t msg = *pMsg;
    if (!msg) {
        return USH_RET_OK;
    }

    free(*pMsg);
    *pMsg = NULL;
    return USH_RET_OK;
}

void
ush_comm_lstnr_sigreg_ack_testpoint(const ush_comm_lstnr_sigreg_ack_t msg) {
    (void)msg;
}
