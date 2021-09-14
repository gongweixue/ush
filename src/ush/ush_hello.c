#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_hello.h"
#include "ush_log.h"


// ack contains the pointers should not be free at destroy function
// be carefulto to manipulate the ack, maybe free already.
typedef struct hello_msg {
    ush_touch_msg_desc desc;
    ush_s8_t           name[USH_HELLO_NAME_LEN_MAX];
    void              *ackSync;
    ush_s32_t          cert;
} * ush_hello_msg_t;

ush_ret_t
ush_hello_create(ush_hello_msg_t    *pHello,
                const ush_char_t    *pName,
                ush_sync_hello_ack_t ack,
                ush_u32_t            cert) {

    ush_assert(pHello && pName && ack);

    ush_assert(strlen(pName) < USH_HELLO_NAME_LEN_MAX);
    ush_assert(strlen(pName) >= USH_HELLO_NAME_LEN_MIN);

    *pHello = NULL;

    ush_hello_msg_t hello = (ush_hello_msg_t)malloc(sizeof(struct hello_msg));
    if (!hello) {
        *pHello = NULL;
        ush_log(USH_LOG_LVL_ERROR, "no mem for hello ack");
        return USH_RET_OUT_OF_MEM;
    }

    hello->desc.catalog = USH_COMM_TOUCH_MSG_CATALOG_HELLO;

    strcpy(hello->name, pName);

    hello->ackSync = ack;
    hello->cert = cert;

    *pHello = hello;

    return USH_RET_OK;
}

ush_ret_t ush_hello_destroy(ush_hello_msg_t *pHello) {
    ush_assert(pHello);
    if (!(*pHello)) {
        ush_log(USH_LOG_LVL_INFO, "hello_msg_t NULL to be destroy\n");
        return USH_RET_OK;
    }

    // just free it self, do not destroy ack
    free(*pHello);
    *pHello = NULL;
    return USH_RET_OK;
}

size_t ush_hello_msg_size() {
    return sizeof(struct hello_msg);
}
