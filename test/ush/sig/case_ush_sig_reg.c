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
    return USH_RET_OK;
}

void test_ush_sig_reg() {

    ush_pipe_t pipe = 0; // magic num
    ush_ret_t ret = OK;
    ush_char_t name[] = "TEST_USH_SIG_REG";

    ret = ush_pipe_create(name, USH_PP_MODE_STD, 0, 0, NULL, 0, &pipe);
    ush_assert(OK == ret);


    ush_sig_reg_conf_t conf0 ={ABC_abc_FP32, onReg, NULL};
    ret = ush_sig_reg(pipe, &conf0);
    ush_assert(OK == ret);

    ret = ush_sig_reg(0, &conf0);
    ush_assert(OK != ret);

    ret = ush_sig_reg(pipe, NULL);
    ush_assert(OK != ret);

    ret = ush_sig_reg(0, NULL);
    ush_assert(OK != ret);

    ush_sig_reg_conf_t conf1 ={USH_SIG_ID_MAX, onReg, NULL};
    ret = ush_sig_reg(pipe, &conf1);
    ush_assert(OK != ret);

    ush_sig_reg_conf_t conf2 ={ABC_abc_FP32, NULL, NULL};
    ret = ush_sig_reg(pipe, &conf2);
    ush_assert(OK == ret);


}
