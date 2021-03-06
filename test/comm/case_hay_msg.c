#include "test-common.h"

#include "ush_define.h"
#include "ush_comm_desc.h"
#include "ush_comm_lstnr.h"
#include "ush_comm_lstnr_hay.h"

static void test_hay_create(void) {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_lstnr_hay_t msg = NULL;

    char buf[1024];
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;

    msg = ush_comm_lstnr_hay_create(sync, 1, 123456, 666);
    ush_assert(NULL != msg);

    msg = ush_comm_lstnr_hay_create(NULL, 1, 123456, 666);
    ush_assert(NULL == msg);

    msg = ush_comm_lstnr_hay_create(sync, 0, 123456, 666);
    ush_assert(NULL == msg);

    msg = ush_comm_lstnr_hay_create(sync, -1, 123456, 666);
    ush_assert(NULL == msg);

    msg = ush_comm_lstnr_hay_create(sync, 1, 0, 666);
    ush_assert(NULL != msg);

    msg = ush_comm_lstnr_hay_create(sync, 1, 0x7FFFFFFF, 666);
    ush_assert(NULL != msg);
}

static void test_hay_destroy(void) {
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
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;

    msg = ush_comm_lstnr_hay_create(sync, 1, 123456, 666);
    ush_assert(NULL != msg);

    ret = ush_comm_lstnr_hay_destroy(&msg);
    ush_assert(OK == ret);
    ush_assert(NULL == msg);
}

static void test_hay_sync_of(void) {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_lstnr_hay_t msg = NULL;

    char buf[1024];
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;

    msg = ush_comm_lstnr_hay_create(sync, 1, 123456, 666);
    ush_assert(NULL != msg);

    ush_connect_sync_t result =
        (ush_connect_sync_t)ush_comm_lstnr_hay_sync_of(msg);
    ush_assert(sync == result);

    // from NULL
    ush_assert(NULL == ush_comm_lstnr_hay_sync_of(NULL));
}

static void test_hay_idx_of(void) {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_lstnr_hay_t msg = NULL;

    char buf[1024];
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;
    ush_connidx_t idx = 1;
    ush_cert_t cert = 123456;

    msg = ush_comm_lstnr_hay_create(sync, idx, cert, 666);
    ush_assert(NULL != msg);

    ush_connidx_t result = ush_comm_lstnr_hay_connidx_of(msg);
    ush_assert(idx == result);

    // from NULL
    ush_assert(USHD_INVALID_CONN_IDX_VALUE == ush_comm_lstnr_hay_connidx_of(NULL));
}

static void test_hay_cert_of(void) {
    ush_ret_t ret = USH_RET_OK;
    (void)ret;
    ush_comm_lstnr_hay_t msg = NULL;

    char buf[1024];
    ush_connect_sync_t sync = (ush_connect_sync_t)buf;
    ush_connidx_t idx = 1;
    ush_cert_t cert = 123456;

    msg = ush_comm_lstnr_hay_create(sync, idx, cert, 666);
    ush_assert(NULL != msg);

    ush_cert_t result = ush_comm_lstnr_hay_cert_of(msg);
    ush_assert(cert == result);

    // from NULL
    ush_assert(USH_INVALID_CERT_VALUE == ush_comm_lstnr_hay_cert_of(NULL));
}

static void test_hay(void) {
    test_hay_create();
    test_hay_destroy();
    test_hay_sync_of();
    test_hay_idx_of();
    test_hay_cert_of();
}
