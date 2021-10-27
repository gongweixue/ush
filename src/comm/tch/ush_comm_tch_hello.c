#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_comm_tch.h"
#include "ush_comm_tch_hello.h"
#include "ush_log.h"


// ack contains the pointers should not be free at destroy function
// be carefulto to manipulate the ack, maybe free already.
typedef struct tch_hello {
    ush_comm_tch_msg_d        desc;
    ush_char_t                name[USH_COMM_CONN_FULL_NAME_LEN_MAX];
    ush_pvoid_t               pAck;
    ush_cert_t                cert;
} USH_COMM_MSG_PACKED * ush_comm_tch_hello_t;

ush_ret_t
ush_comm_tch_hello_create(ush_comm_tch_hello_t    *pHello,
                          const ush_char_t        *name,
                          ush_sync_hello_ack_t    *pAck,
                          ush_cert_t               cert) {

    if (!pHello || !name || !pAck) {
        if (pHello) {*pHello = NULL;}
        ush_log(LOG_LVL_FATAL, "param NULL");
        return USH_RET_WRONG_PARAM;
    }

    *pHello = NULL;

    // name overflow
    if (strlen(name) >= sizeof(((ush_comm_tch_hello_t)0)->name)) {
        ush_log(LOG_LVL_FATAL, "name overflow");
        return USH_RET_WRONG_PARAM;
    }
    if (strlen(name) == 0) {
        ush_log(LOG_LVL_FATAL, "name len = 0");
        return USH_RET_WRONG_PARAM;
    }


    ush_comm_tch_hello_t tmp =
        (ush_comm_tch_hello_t)malloc(sizeof(struct tch_hello));
    if (!tmp) {
        *pHello = NULL;
        ush_log(LOG_LVL_FATAL, "no mem for hello");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_DETAIL, "allocate memory for msg %p", tmp);

    tmp->desc.desc.port = USH_COMM_PORT_TCH;
    tmp->desc.catalog   = USH_COMM_TCH_MSG_CATALOG_HELLO;

    strcpy(tmp->name, name);

    tmp->pAck = (ush_pvoid_t)pAck;
    tmp->cert = cert;

    *pHello = tmp;

    return USH_RET_OK;
}

ush_ret_t
ush_comm_tch_hello_destroy(ush_comm_tch_hello_t *pHello) {
    if (!pHello || !(*pHello)) {
        ush_log(LOG_LVL_INFO, "hello_msg_t NULL to be destroy");
        return USH_RET_OK;
    }

    ush_log(LOG_LVL_DETAIL, "free memory for msg %p", *pHello);
    // just free it self, do not destroy ack
    free(*pHello);
    *pHello = NULL;

    return USH_RET_OK;
}

size_t
ush_comm_tch_hello_sizeof(void) {
    return sizeof(struct tch_hello);
}

const ush_char_t *
ush_comm_tch_hello_name_of(const ush_comm_tch_hello_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return NULL;
    }
    return msg->name;
}

ush_pvoid_t
ush_comm_tch_hello_ack_of(const ush_comm_tch_hello_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return NULL;
    }
    return msg->pAck;
}

ush_cert_t
ush_comm_tch_hello_cert_of(const ush_comm_tch_hello_t msg) {
    ush_assert(msg);
    return msg->cert;
}

void
ush_comm_tch_hello_testpoint(const ush_comm_tch_hello_t msg) {
    (void)msg; // breakpoint reached here to inspect the data
}
