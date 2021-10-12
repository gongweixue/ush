#include "test-common.h"
#include "ush_pipe_pub.h"
#include "ush_sig_pub.h"
#include "ush_sig_enum.h"
#include "ush_string.h"

ush_ret_t onReg(ush_pipe_t         pipe,
                ush_sig_id_t       id,
                ush_bool_t         success,
                const ush_pvoid_t *pParams)
{
    ush_pipe_t pp = pipe;
    ush_sig_id_t sigid = id;
    ush_bool_t ok = success;
    return USH_RET_OK;
}

void test_ush_sig_reg() {

    ush_pipe_t pipe = 0; // magic num
    ush_ret_t ret = OK;
    ush_char_t name[] = "TEST_USH_SIG_REG";

    ret = ush_pipe_create(name, USH_PP_MODE_STD, 0, 0, NULL, 0, &pipe);
    ush_assert(OK == ret);


    ush_sig_reg_conf_t conf ={ABC_abc_FP32, onReg, NULL};
    ret = ush_sig_reg(pipe, &conf);
    ush_assert(OK == ret);
}
