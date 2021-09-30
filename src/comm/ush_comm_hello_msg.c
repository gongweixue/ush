#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_comm_hello_msg.h"
#include "ush_log.h"


// ack contains the pointers should not be free at destroy function
// be carefulto to manipulate the ack, maybe free already.
typedef struct hello_msg {
    ush_touch_msg_description desc;
    ush_char_t         name[USH_COMM_CONN_NAME_LEN_MAX];
    ush_vptr_t        *ackSync;
    ush_s32_t          cert;
} USH_COMM_MSG_PACKED * ush_comm_hello_msg_t;

ush_ret_t
ush_comm_hello_msg_create(ush_comm_hello_msg_t    *pHello,
                          const ush_char_t        *name,
                          ush_sync_hello_ack_t     ack,
                          ush_s32_t                cert) {

    if (!pHello || !name || !ack) {
        if (pHello) *pHello = NULL;
        ush_log(LOG_LVL_FATAL, "param NULL");
        return USH_RET_WRONG_PARAM;
    }

    *pHello = NULL;

    // name overflow
    if (strlen(name) >= sizeof(((ush_comm_hello_msg_t)0)->name)) {
        ush_log(LOG_LVL_FATAL, "name overflow");
        return USH_RET_WRONG_PARAM;
    }
    if (strlen(name) == 0) {
        ush_log(LOG_LVL_FATAL, "name len = 0");
        return USH_RET_WRONG_PARAM;
    }


    ush_comm_hello_msg_t tmp =
        (ush_comm_hello_msg_t)malloc(sizeof(struct hello_msg));
    if (!tmp) {
        *pHello = NULL;
        ush_log(LOG_LVL_FATAL, "no mem for hello");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_DETAIL, "allocate memory for msg %p", tmp);

    tmp->desc.catalog = USH_COMM_TOUCH_MSG_CATALOG_HELLO;

    strcpy(tmp->name, name);

    tmp->ackSync = (ush_vptr_t)ack;
    tmp->cert = cert;

    *pHello = tmp;

    return USH_RET_OK;
}

ush_ret_t
ush_comm_hello_msg_destroy(ush_comm_hello_msg_t *pHello) {
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
ush_comm_hello_msg_sizeof() {
    return sizeof(struct hello_msg);
}

const ush_char_t *
ush_comm_hello_msg_name_of(const ush_comm_hello_msg_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return NULL;
    }
    return msg->name;
}

const ush_vptr_t
ush_comm_hello_msg_ack_of(const ush_comm_hello_msg_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return NULL;
    }
    return msg->ackSync;
}

ush_s32_t
ush_comm_hello_msg_cert_of(const ush_comm_hello_msg_t msg) {
    ush_assert(msg);
    return msg->cert;
}

void
ush_comm_hello_msg_testpoint(const ush_comm_hello_msg_t msg) {
    (void)msg; // breakpoint reached here to inspect the data
}
