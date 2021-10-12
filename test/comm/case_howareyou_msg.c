#include "test-common.h"

#include "ush_comm.h"

static void test_howareyou_create() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_howareyou_msg_t msg = NULL;

    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;

    msg = ush_comm_howareyou_msg_create(ack, 1, 123456);
    ush_assert(NULL != msg);

    msg = ush_comm_howareyou_msg_create(NULL, 1, 123456);
    ush_assert(NULL == msg);

    msg = ush_comm_howareyou_msg_create(ack, 0, 123456);
    ush_assert(NULL != msg);

    msg = ush_comm_howareyou_msg_create(ack, -1, 123456);
    ush_assert(NULL == msg);

    msg = ush_comm_howareyou_msg_create(ack, 1, 0);
    ush_assert(NULL != msg);

    msg = ush_comm_howareyou_msg_create(ack, 1, 0xFFFFFFFF);
    ush_assert(NULL != msg);
}

static void test_howareyou_destroy() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;

    // destroy NULL
    ret = ush_comm_howareyou_msg_destroy(NULL);
    ush_assert(OK == ret);

    ush_comm_howareyou_msg_t msg = NULL;
    ret = ush_comm_howareyou_msg_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);


    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;

    msg = ush_comm_howareyou_msg_create(ack, 1, 123456);
    ush_assert(NULL != msg);

    ret = ush_comm_howareyou_msg_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);
}

static void test_howareyou_sizeof() {
    ush_assert(20 == ush_comm_howareyou_msg_sizeof());
}

static void test_howareyou_ack_of() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_howareyou_msg_t msg = NULL;

    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;

    msg = ush_comm_howareyou_msg_create(ack, 1, 123456);
    ush_assert(NULL != msg);

    ush_sync_hello_ack_t result = ush_comm_howareyou_msg_ack_of(msg);
    ush_assert(ack == result);

    // from NULL
    ush_assert(NULL == ush_comm_howareyou_msg_ack_of(NULL));
}

static void test_howareyou_idx_of() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_howareyou_msg_t msg = NULL;

    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;
    ush_s32_t idx = 1;
    ush_s32_t cert = 123456;

    msg = ush_comm_howareyou_msg_create(ack, idx, cert);
    ush_assert(NULL != msg);

    ush_s32_t result = ush_comm_howareyou_msg_remote_idx_of(msg);
    ush_assert(idx == result);

    // from NULL
    ush_assert(0 == ush_comm_howareyou_msg_remote_idx_of(NULL));
}

static void test_howareyou_cert_of() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_howareyou_msg_t msg = NULL;

    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;
    ush_s32_t idx = 1;
    ush_s32_t cert = 123456;

    msg = ush_comm_howareyou_msg_create(ack, idx, cert);
    ush_assert(NULL != msg);

    ush_s32_t result = ush_comm_howareyou_msg_cert_of(msg);
    ush_assert(cert == result);

    // from NULL
    ush_assert(0xFFFFFFFF == ush_comm_howareyou_msg_cert_of(NULL));
}

void test_howareyou() {
    test_howareyou_create();
    test_howareyou_destroy();
    test_howareyou_sizeof();
    test_howareyou_ack_of();
    test_howareyou_idx_of();
    test_howareyou_cert_of();
}
