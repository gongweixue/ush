#include "ush_comm_hello_msg.h"
#include "ush_assert.h"

void test_hello() {
    ush_ret_t ret = USH_RET_OK;
    ush_comm_hello_msg_t msg = NULL;
    ret = ush_comm_hello_msg_create(&msg, "testname", "ack", 123456);
    ush_assert(USH_RET_OK == ret);
}
