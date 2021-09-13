
#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_hello.h"
#include "ush_log.h"

typedef struct hello_msg {
    ush_touch_msg_desc desc;
    ush_s8_t           name[USH_HELLO_NAME_LEN_MAX];
    void              *ackSync;
    ush_s32_t          cert;
} * ush_hello_msg_t;

ush_ret_t
ush_hello_create(ush_hello_msg_t     hello,
                const ush_char_t    *pName,
                ush_sync_hello_ack_t ack) {

    assert(hello && pName && ack);

    assert(strlen(pName) < USH_HELLO_NAME_LEN_MAX);
    assert(strlen(pName) >= USH_HELLO_NAME_LEN_MIN);

    hello = (ush_hello_msg_t)malloc(sizeof(struct hello_msg));
    if (!hello) {
        ush_log(USH_LOG_LVL_ERROR, "no mem for hello ack");
        return USH_RET_OUT_OF_MEM;
    }

    hello->desc.catalog = USH_COMM_TOUCH_MSG_CATALOG_HELLO;

    strcpy(hello->name, pName);

    hello->ackSync = ack;

    srand((unsigned int)pName[0] + pName[1] + pName[2]); // first 3 bytes
    hello->cert = rand();
    ush_log(USH_LOG_LVL_INFO, "random cert %d\n", hello->cert);

    return USH_RET_OK;
}

ush_ret_t ush_hello_destroy(ush_hello_msg_t hello) {
    if (!hello) {
        return USH_RET_OK;
    }

    free(hello);
    return USH_RET_OK;
}

size_t ush_hello_msg_size() {
    return sizeof(struct hello_msg);
}
