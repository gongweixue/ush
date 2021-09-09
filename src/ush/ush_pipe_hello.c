#include "ush_pipe_hello.h"
#include "assert.h"
#include "stdlib.h"
#include "string.h"

ush_ret_t
ush_pipe_hello_load(ush_pipe_msg_hello_t *pHello,
                    const char*           pName,
                    sync_hello_ack_t     *pAck) {
    assert(pHello && pName);
    assert(strlen(pName) < USH_COMM_TOUCH_Q_HELLO_NAME_LEN_MAX);
    assert(strlen(pName) >= USH_COMM_TOUCH_Q_HELLO_NAME_LEN_MIN);

    pHello->desc.catalog = USH_COMM_TOUCH_MSG_CATALOG_HELLO;

    strcpy(pHello->name, pName);

    pHello->ackSync = pAck;

    srand((unsigned int)pName[0] + pName[1] + pName[2]); // first 3 bytes
    pHello->cert = rand();

    return USH_RET_OK;
}
