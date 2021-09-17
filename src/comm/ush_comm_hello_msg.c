#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_comm_hello_msg.h"
#include "ush_log.h"


// ack contains the pointers should not be free at destroy function
// be carefulto to manipulate the ack, maybe free already.
typedef struct ush_comm_hello_msg {
    ush_touch_msg_desc desc;
    ush_s8_t           name[USH_COMM_HELLO_MSG_NAME_LEN_MAX];
    void              *ackSync;
    ush_s32_t          cert;
} * ush_comm_hello_msg_t USH_COMM_TOUCH_Q_MSG_ALIGNMENT;

ush_ret_t
ush_comm_hello_msg_create(ush_comm_hello_msg_t    *pHello,
                          const ush_char_t        *pName,
                          void                    *pAck,
                          ush_u32_t                cert) {

    ush_assert(pHello && pName && pAck);

    ush_assert(strlen(pName) < USH_COMM_HELLO_MSG_NAME_LEN_MAX);
    ush_assert(strlen(pName) >= USH_COMM_HELLO_MSG_NAME_LEN_MIN);

    *pHello = NULL;

    ush_comm_hello_msg_t tmp =
        (ush_comm_hello_msg_t)malloc(sizeof(struct ush_comm_hello_msg));
    if (!tmp) {
        *pHello = NULL;
        ush_log(LOG_LVL_FATAL, "no mem for hello");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_DETAIL, "allocate memory for msg %p", tmp);

    tmp->desc.catalog = USH_COMM_TOUCH_MSG_CATALOG_HELLO;

    strcpy(tmp->name, pName);

    tmp->ackSync = pAck;
    tmp->cert = cert;

    *pHello = tmp;

    return USH_RET_OK;
}

ush_ret_t
ush_comm_hello_msg_destroy(ush_comm_hello_msg_t *pHello) {
    ush_assert(pHello);
    if (!(*pHello)) {
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
ush_comm_hello_msg_size() {
    return sizeof(struct ush_comm_hello_msg);
}
