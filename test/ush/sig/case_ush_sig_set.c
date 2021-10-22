#include "test-common.h"
#include "ush_pipe_pub.h"
#include "ush_sig_pub.h"
#include "ush_sig_id.h"
#include "ush_string.h"

static void test_ush_sig_set(void) {

    ush_pipe_t pipe = USH_INVALID_PIPE; // magic num
    ush_ret_t ret = OK;
    ush_char_t name[] = "TEST_USH_SIG_REG";

    ret = ush_pipe_create(name, USH_PP_MODE_STD, 0, 0, NULL, 0, &pipe);
    ush_assert(OK == ret);


    FP32 f = 1.0f;
    ret = ush_sig_set(pipe, USH_SIG_ID_ABC_abc_FP32, &f);
    ush_assert(OK == ret);
}
