#include "assert.h"
#include "stdlib.h"
#include "string.h"

#include "ush_hello.h"
#include "ush_log.h"

ush_ret_t
ush_hello_composite(ush_hello_msg_t      *pHello,
                    const char           *pName,
                    ush_sync_hello_ack_t *pAck) {

    assert(pHello && pName && pAck);

    assert(strlen(pName) < USH_HELLO_NAME_LEN_MAX);
    assert(strlen(pName) >= USH_HELLO_NAME_LEN_MIN);

    pHello->desc.catalog = USH_COMM_TOUCH_MSG_CATALOG_HELLO;

    strcpy(pHello->name, pName);

    pHello->ackSync = pAck;

    srand((unsigned int)pName[0] + pName[1] + pName[2]); // first 3 bytes
    pHello->cert = rand();
    ush_log(USH_LOG_LVL_INFO, "random cert %d\n", pHello->cert);

    return USH_RET_OK;
}
