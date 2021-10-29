#include "stdlib.h"

#include "ush_assert.h"
#include "ush_comm_lstnr.h"
#include "ush_comm_lstnr_hay.h"
#include "ush_log.h"

typedef struct lstnr_hay_s {
    ush_comm_lstnr_msg_d         desc;
    ush_sync_hello_ack_t        *sync;
    ush_connidx_t                connidx;
    ush_cert_t                   cert;
} USH_COMM_MSG_PACKED * ush_comm_lstnr_hay_t;


ush_comm_lstnr_hay_t
ush_comm_lstnr_hay_create(ush_pvoid_t   sync,
                          ush_connidx_t idx,
                          ush_cert_t    cert) {
    if (!sync || idx <= USHD_INVALID_CONN_IDX_VALUE) {
        ush_log(LOG_LVL_ERROR, "null ptr of sync or invalid idx");
        return NULL;
    }
    ush_comm_lstnr_hay_t ret =
        (ush_comm_lstnr_hay_t)malloc(sizeof(struct lstnr_hay_s));

    if (!ret) {
        ush_log(LOG_LVL_ERROR, "hay msg mem alloc failed.");
        return NULL;
    }

    ret->desc.desc.port = USH_COMM_PORT_LSTNR;
    ret->desc.catalog   = USH_COMM_LSTNR_MSG_CATALOG_HAY;
    ret->sync           = (ush_sync_hello_ack_t*)sync;
    ret->connidx        = idx;
    ret->cert           = cert;

    return ret;
}

ush_sync_hello_ack_t *
ush_comm_lstnr_hay_ack_of(ush_comm_lstnr_hay_t msg) {
    if (!msg) {
        return NULL;
    }

    return msg->sync;
}

ush_connidx_t
ush_comm_lstnr_hay_connidx_of(ush_comm_lstnr_hay_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_INFO, "get idx from NULL, return 0(invalid idx)");
        return 0;
    }

    return msg->connidx;
}

ush_cert_t
ush_comm_lstnr_hay_cert_of(ush_comm_lstnr_hay_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_INFO, "get idx from NULL, return 0xFFFFFFFF(invald)");
        return USH_INVALID_CERT_VALUE;
    }

    return msg->cert;
}

ush_size_t
ush_comm_lstnr_hay_sizeof(void) {
    return sizeof(struct lstnr_hay_s);
}

ush_ret_t
ush_comm_lstnr_hay_destroy(ush_comm_lstnr_hay_t *pMsg) {
    if (!pMsg) {
        return USH_RET_OK;
    }

    ush_comm_lstnr_hay_t msg = *pMsg;
    if (!msg) {
        return USH_RET_OK;
    }

    free(*pMsg);
    *pMsg = NULL;
    return USH_RET_OK;
}

void
ush_comm_lstnr_hay_testpoint(const ush_comm_lstnr_hay_t msg) {
    (void)msg;
}
