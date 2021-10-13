#include "test-common.h"

#include "ush_comm_port.h"
#include "lstnr/ush_comm_lstnr.h"
#include "lstnr/ush_comm_lstnr_hay.h"

static void test_hay_create() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_lstnr_hay_t msg = NULL;

    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;

    msg = ush_comm_lstnr_hay_create(ack, 1, 123456);
    ush_assert(NULL != msg);

    msg = ush_comm_lstnr_hay_create(NULL, 1, 123456);
    ush_assert(NULL == msg);

    msg = ush_comm_lstnr_hay_create(ack, 0, 123456);
    ush_assert(NULL != msg);

    msg = ush_comm_lstnr_hay_create(ack, -1, 123456);
    ush_assert(NULL == msg);

    msg = ush_comm_lstnr_hay_create(ack, 1, 0);
    ush_assert(NULL != msg);

    msg = ush_comm_lstnr_hay_create(ack, 1, 0xFFFFFFFF);
    ush_assert(NULL != msg);
}

static void test_hay_destroy() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;

    // destroy NULL
    ret = ush_comm_lstnr_hay_destroy(NULL);
    ush_assert(OK == ret);

    ush_comm_lstnr_hay_t msg = NULL;
    ret = ush_comm_lstnr_hay_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);


    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;

    msg = ush_comm_lstnr_hay_create(ack, 1, 123456);
    ush_assert(NULL != msg);

    ret = ush_comm_lstnr_hay_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);
}

static void test_hay_ack_of() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_lstnr_hay_t msg = NULL;

    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;

    msg = ush_comm_lstnr_hay_create(ack, 1, 123456);
    ush_assert(NULL != msg);

    ush_sync_hello_ack_t result = ush_comm_lstnr_hay_ack_of(msg);
    ush_assert(ack == result);

    // from NULL
    ush_assert(NULL == ush_comm_lstnr_hay_ack_of(NULL));
}

static void test_hay_idx_of() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_lstnr_hay_t msg = NULL;

    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;
    ush_s32_t idx = 1;
    ush_s32_t cert = 123456;

    msg = ush_comm_lstnr_hay_create(ack, idx, cert);
    ush_assert(NULL != msg);

    ush_s32_t result = ush_comm_lstnr_hay_remote_idx_of(msg);
    ush_assert(idx == result);

    // from NULL
    ush_assert(0 == ush_comm_lstnr_hay_remote_idx_of(NULL));
}

static void test_hay_cert_of() {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_lstnr_hay_t msg = NULL;

    char buf[1024];
    ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)buf;
    ush_s32_t idx = 1;
    ush_s32_t cert = 123456;

    msg = ush_comm_lstnr_hay_create(ack, idx, cert);
    ush_assert(NULL != msg);

    ush_s32_t result = ush_comm_lstnr_hay_cert_of(msg);
    ush_assert(cert == result);

    // from NULL
    ush_assert(0xFFFFFFFF == ush_comm_lstnr_hay_cert_of(NULL));
}

void test_hay() {
    test_hay_create();
    test_hay_destroy();
    test_hay_ack_of();
    test_hay_idx_of();
    test_hay_cert_of();
}