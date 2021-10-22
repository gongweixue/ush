#include "test-common.h"
#include "ush_pipe_pub.h"
#include "ush_string.h"


static void test_ush_pipe_create(void) {
    ush_pipe_t pipe = 0x55AA; // magic num
    ush_ret_t ret = OK;
    ush_char_t name[] = "TEST_USH_PIPE_CREATE";

    ret = ush_pipe_create(NULL, USH_PP_MODE_STD, 0, 0, NULL, 0, &pipe);
    ush_assert(OK != ret);
    ush_assert(0 == pipe);

    ret = ush_pipe_create(name, USH_PP_MODE_STD, 0, 0, NULL, 0, &pipe);
    ush_assert(OK == ret);
    ush_assert(0 != pipe);

    ret = ush_pipe_create(name, USH_PP_MODE_MAX_GUARD, 0, 0, NULL, 0, &pipe);
    ush_assert(OK != ret);
    ush_assert(0 == pipe);

    ret = ush_pipe_create(name, USH_PP_MODE_STD, 0, 3, NULL, 0, &pipe);
    ush_assert(OK == ret);
    ush_assert(0 != pipe);

    ret = ush_pipe_create(name, USH_PP_MODE_STD, 0, 0, NULL, 0, NULL);
    ush_assert(OK != ret);

    // name too long
    ush_char_t longname[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    ret = ush_pipe_create(longname, USH_PP_MODE_STD, 0, 0, NULL, 0, &pipe);
    ush_assert(OK != ret);
    ush_assert(0 == pipe);
}
