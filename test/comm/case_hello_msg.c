#include "test-common.h"

#include "ush_comm_tch_hello.h"

static void test_hello_create(void) {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_tch_hello_t msg = NULL;

    char buf[1024];
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;

    ret = ush_comm_tch_hello_create(&msg, "testname", sync, 123456, NULL);
    ush_assert(OK == ret);

    ret = ush_comm_tch_hello_create(&msg, "testname", sync, 0, NULL);
    ush_assert(OK == ret);

    ret = ush_comm_tch_hello_create(&msg, "adsd", sync, 0x7FFFFFFF, NULL);
    ush_assert(OK == ret);

    //exception
    ret = ush_comm_tch_hello_create(NULL, "abc", sync, 0, NULL);
    ush_assert(OK != ret);

    ret = ush_comm_tch_hello_create(&msg, "", sync, 0, NULL);
    ush_assert(OK != ret);
    ush_assert(NULL == msg);

    char name[] = // 64 bytes
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    ret = ush_comm_tch_hello_create(&msg, name, sync, 123456, NULL);
    ush_assert(OK != ret);
    ush_assert(NULL == msg);

    ret = ush_comm_tch_hello_create(&msg, name, NULL, 123456, NULL);
    ush_assert(OK != ret);
    ush_assert(NULL == msg);
}

static void test_hello_destroy(void) {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_tch_hello_t msg = NULL;
    char buf[1024];
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;

    ret = ush_comm_tch_hello_create(&msg, "testname", sync, 123456, NULL);
    ush_assert(OK == ret);

    ret = ush_comm_tch_hello_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);

    msg = NULL;
    ret = ush_comm_tch_hello_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);

    ret = ush_comm_tch_hello_destroy(NULL);
    ush_assert(OK == ret);
}

static void test_hello_name_of(void) {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_tch_hello_t msg = NULL;
    char buf[1024];
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;
    char name[] = "testname";
    ush_cert_t cert = 123456;

    ret = ush_comm_tch_hello_create(&msg, name, sync, cert, NULL);
    ush_assert(OK == ret);
    ush_assert(NULL != msg);

    const ush_char_t *result = ush_comm_tch_hello_name_of(msg);
    ush_assert(0 == strcmp(result, name));

    ret = ush_comm_tch_hello_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);

    // get from NULL
    ush_assert(NULL == ush_comm_tch_hello_name_of(NULL));
}

static void test_hello_sync_of(void) {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_tch_hello_t msg = NULL;
    char buf[1024];
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;
    char name[] = "testname";
    ush_cert_t cert = 123456;

    ret = ush_comm_tch_hello_create(&msg, name, sync, cert, NULL);
    ush_assert(OK == ret);
    ush_assert(NULL != msg);

    ush_connect_sync_t *result = ush_comm_tch_hello_sync_of(msg);
    ush_assert((ush_connect_sync_t)result == sync);

    ret = ush_comm_tch_hello_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);

    // get from NULL
    ush_assert(NULL == ush_comm_tch_hello_sync_of(NULL));
}

static void test_hello_cert_of(void) {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_tch_hello_t msg = NULL;
    char buf[1024];
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;
    char name[] = "testname";

    { // case 1
        ush_cert_t cert = 123456;

        ret = ush_comm_tch_hello_create(&msg, name, sync, cert, NULL);
        ush_assert(OK == ret);
        ush_assert(NULL != msg);

        ush_connect_sync_t *result = ush_comm_tch_hello_sync_of(msg);
        ush_assert((ush_connect_sync_t)result == sync);

        ret = ush_comm_tch_hello_destroy(&msg);
        ush_assert(OK == ret);
        ush_assert(NULL == msg);
    }

    { // case 2
        ush_cert_t cert = 0;

        ret = ush_comm_tch_hello_create(&msg, name, sync, cert, NULL);
        ush_assert(OK == ret);
        ush_assert(NULL != msg);

        ush_pvoid_t *result = ush_comm_tch_hello_sync_of(msg);
        ush_assert((ush_connect_sync_t)result == sync);

        ret = ush_comm_tch_hello_destroy(&msg);
        ush_assert(OK == ret);
        ush_assert(NULL == msg);
    }

    { // case 3
        ush_cert_t cert = 0x7FFFFFFF;

        ret = ush_comm_tch_hello_create(&msg, name, sync, cert, NULL);
        ush_assert(OK == ret);
        ush_assert(NULL != msg);

        ush_pvoid_t *result = ush_comm_tch_hello_sync_of(msg);
        ush_assert((ush_connect_sync_t)result == sync);

        ret = ush_comm_tch_hello_destroy(&msg);
        ush_assert(OK == ret);
        ush_assert(NULL == msg);
    }

    // get from NULL
    ush_assert(NULL == ush_comm_tch_hello_sync_of(NULL));
}

static void test_hello(void) {
    test_hello_create();
    test_hello_destroy();
    test_hello_name_of();
    test_hello_sync_of();
    test_hello_cert_of();
}
