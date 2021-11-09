#include "stdlib.h"

#include "ush_type_pub.h"
#include "ush_log.h"

#include "ush_comm_tch.h"
#include "ush_comm_tch_goodbye.h"

typedef struct tch_goodbye_s {
    ush_comm_tch_msg_d        desc;
    ush_connidx_t             connidx;
    ush_cert_t                cert;
} USH_COMM_MSG_PACKED * ush_comm_tch_goodbye_t;

ush_ret_t
ush_comm_tch_goodbye_create(ush_comm_tch_goodbye_t *pGoodbye,
                            ush_connidx_t           connidx,
                            ush_cert_t              cert) {
    if (!pGoodbye) {
        ush_log(LOG_LVL_FATAL, "param NULL");
        return USH_RET_WRONG_PARAM;
    }

    *pGoodbye = (ush_comm_tch_goodbye_t)malloc(sizeof(struct tch_goodbye_s));
    if (!(*pGoodbye)) {
        ush_log(LOG_LVL_FATAL, "no mem for goodbye");
        return USH_RET_OUT_OF_MEM;
    }

    (*pGoodbye)->desc.desc.port = USH_COMM_PORT_TCH;
    (*pGoodbye)->desc.catalog   = USH_COMM_TCH_MSG_CATALOG_GOODBYE;
    (*pGoodbye)->connidx        = connidx;
    (*pGoodbye)->cert           = cert;

    return USH_RET_OK;
}

ush_ret_t
ush_comm_tch_goodbye_destroy(ush_comm_tch_goodbye_t *pGoodbye) {
    if (!pGoodbye || !(*pGoodbye)) {
        ush_log(LOG_LVL_INFO, "goodbye_t NULL to be destroy");
        return USH_RET_OK;
    }

    // just free it self, do not destroy sync
    free(*pGoodbye);
    *pGoodbye = NULL;

    return USH_RET_OK;
}

size_t
ush_comm_tch_goodbye_sizeof(void) {
    return sizeof(struct tch_goodbye_s);
}

ush_connidx_t
ush_comm_tch_goodbye_connidx_of(const ush_comm_tch_goodbye_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return USHD_INVALID_CONN_IDX_VALUE;
    }
    return msg->connidx;
}

ush_cert_t
ush_comm_tch_goodbye_cert_of(const ush_comm_tch_goodbye_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return USH_INVALID_CERT_VALUE;
    }
    return msg->cert;
}

void
ush_comm_tch_goodbye_testpoint(const ush_comm_tch_goodbye_t msg) {
    (void)msg; // breakpoint reached here to inspect the data
}
