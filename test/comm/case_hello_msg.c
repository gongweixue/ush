#include "test-common.h"

#include "ush_comm_hello_msg.h"

static void test_hello_create() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_hello_msg_t msg = NULL;

    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;

    ret = ush_comm_hello_msg_create(&msg, "testname", ack, 123456);
    ush_assert(OK == ret);

    ret = ush_comm_hello_msg_create(&msg, "testname", ack, 0);
    ush_assert(OK == ret);

    ret = ush_comm_hello_msg_create(&msg, "adsd", ack, 0xFFFFFFFF);
    ush_assert(OK == ret);

    //exception
    ret = ush_comm_hello_msg_create(NULL, "abc", ack, 0);
    ush_assert(OK != ret);

    ret = ush_comm_hello_msg_create(&msg, "", ack, 0);
    ush_assert(OK != ret);
    ush_assert(NULL == msg);

    char name[] = // 64 bytes
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    ret = ush_comm_hello_msg_create(&msg, name, ack, 123456);
    ush_assert(OK != ret);
    ush_assert(NULL == msg);

    ret = ush_comm_hello_msg_create(&msg, name, NULL, 123456);
    ush_assert(OK != ret);
    ush_assert(NULL == msg);
}

static void test_hello_destroy() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_hello_msg_t msg = NULL;
    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;

    ret = ush_comm_hello_msg_create(&msg, "testname", ack, 123456);
    ush_assert(OK == ret);

    ret = ush_comm_hello_msg_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);

    msg = NULL;
    ret = ush_comm_hello_msg_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);

    ret = ush_comm_hello_msg_destroy(NULL);
    ush_assert(OK == ret);
}

static void test_hello_sizeof() {
    ush_assert(80 == ush_comm_hello_msg_sizeof());
}

static void test_hello_name_of() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_hello_msg_t msg = NULL;
    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;
    char name[] = "testname";
    ush_s32_t cert = 123456;

    ret = ush_comm_hello_msg_create(&msg, name, ack, cert);
    ush_assert(OK == ret);
    ush_assert(NULL != msg);

    const ush_char_t *result = ush_comm_hello_msg_name_of(msg);
    ush_assert(0 == strcmp(result, name));

    ret = ush_comm_hello_msg_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);

    // get from NULL
    ush_assert(NULL == ush_comm_hello_msg_name_of(NULL));
}

static void test_hello_ack_of() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_hello_msg_t msg = NULL;
    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;
    char name[] = "testname";
    ush_s32_t cert = 123456;

    ret = ush_comm_hello_msg_create(&msg, name, ack, cert);
    ush_assert(OK == ret);
    ush_assert(NULL != msg);

    const ush_pvoid_t *result = ush_comm_hello_msg_ack_of(msg);
    ush_assert((ush_sync_hello_ack_t)result == ack);

    ret = ush_comm_hello_msg_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);

    // get from NULL
    ush_assert(NULL == ush_comm_hello_msg_ack_of(NULL));
}

static void test_hello_cert_of() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_hello_msg_t msg = NULL;
    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;
    char name[] = "testname";

    { // case 1
        ush_s32_t cert = 123456;

        ret = ush_comm_hello_msg_create(&msg, name, ack, cert);
        ush_assert(OK == ret);
        ush_assert(NULL != msg);

        const ush_pvoid_t *result = ush_comm_hello_msg_ack_of(msg);
        ush_assert((ush_sync_hello_ack_t)result == ack);

        ret = ush_comm_hello_msg_destroy(&msg);
        ush_assert(OK == ret);
        ush_assert(NULL == msg);
    }

    { // case 2
        ush_s32_t cert = 0;

        ret = ush_comm_hello_msg_create(&msg, name, ack, cert);
        ush_assert(OK == ret);
        ush_assert(NULL != msg);

        const ush_pvoid_t *result = ush_comm_hello_msg_ack_of(msg);
        ush_assert((ush_sync_hello_ack_t)result == ack);

        ret = ush_comm_hello_msg_destroy(&msg);
        ush_assert(OK == ret);
        ush_assert(NULL == msg);
    }

    { // case 3
        ush_s32_t cert = 0xFFFFFFFF;

        ret = ush_comm_hello_msg_create(&msg, name, ack, cert);
        ush_assert(OK == ret);
        ush_assert(NULL != msg);

        const ush_pvoid_t *result = ush_comm_hello_msg_ack_of(msg);
        ush_assert((ush_sync_hello_ack_t)result == ack);

        ret = ush_comm_hello_msg_destroy(&msg);
        ush_assert(OK == ret);
        ush_assert(NULL == msg);
    }

    // get from NULL
    ush_assert(NULL == ush_comm_hello_msg_ack_of(NULL));
}

void test_hello() {
    test_hello_create();
    test_hello_destroy();
    test_hello_sizeof();
    test_hello_name_of();
    test_hello_ack_of();
    test_hello_cert_of();
}
