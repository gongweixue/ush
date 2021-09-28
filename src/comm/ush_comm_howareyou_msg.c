#include "stdlib.h"

#include "ush_assert.h"
#include "ush_log.h"
#include "ush_def_pub.h"
#include "ush_comm_howareyou_msg.h"

typedef struct howareyou_msg {
    ush_listener_msg_description desc;
    ush_vptr_t              sync;
    ush_s32_t               remote_idx;
    ush_s32_t               cert;
} * ush_comm_howareyou_msg_t USH_COMM_MSG_ALIGNMENT;


ush_comm_howareyou_msg_t
ush_comm_howareyou_msg_create(ush_vptr_t sync, ush_s32_t idx, ush_s32_t cert) {
    ush_comm_howareyou_msg_t ret =
        (ush_comm_howareyou_msg_t)malloc(sizeof(struct howareyou_msg));

    if (!ret) {
        ush_log(LOG_LVL_ERROR, "howareyou msg mem alloc failed.");
        return NULL;
    }

    ret->desc.catalog = USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU;
    ret->sync         = sync;
    ret->remote_idx   = idx;
    ret->cert         = cert;

    return ret;
}

ush_vptr_t
ush_comm_howareyou_msg_ack_of(ush_comm_howareyou_msg_t msg) {
    ush_assert(msg);
    if (!msg) {
        return NULL;
    }

    return msg->sync;
}

ush_s32_t
ush_comm_howareyou_msg_remote_idx_of(ush_comm_howareyou_msg_t msg) {
    ush_assert(msg);
    if (!msg) {
        return 0;
    }

    return msg->remote_idx;
}

ush_s32_t
ush_comm_howareyou_msg_cert_of(ush_comm_howareyou_msg_t msg) {
    ush_assert(msg);
    if (!msg) {
        return USH_INVALID_CERT_VALUE_DEFAULT;
    }

    return msg->cert;
}

ush_size_t
ush_comm_howareyou_msg_sizeof() {
    return sizeof(struct howareyou_msg);
}

ush_ret_t
ush_comm_howareyou_msg_destroy(ush_comm_howareyou_msg_t *pMsg) {
    if (!pMsg || *pMsg) {
        return USH_RET_OK;
    }

    free(*pMsg);
    pMsg = NULL;
    return USH_RET_OK;
}

void
ush_comm_howareyou_msg_testpoint(ush_comm_howareyou_msg_t msg) {
    (void)msg;
}
