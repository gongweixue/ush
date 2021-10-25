#include "test-common.h"
#include "ush_pipe_pub.h"
#include "ush_sig_pub.h"
#include "ush_sig_id.h"
#include "ush_string.h"

#include "case_ush_sig_reg.h"

#include "pthread.h"
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
static int flag_done = 0;
static int flag_rcv  = 0;

static ush_pipe_t sPipe = USH_INVALID_PIPE; // magic num

static ush_ret_t onReg(ush_pipe_t pp, ush_sig_id_t id, ush_bool_t success) {
    ush_assert(pp == sPipe);
    ush_assert(USH_SIG_ID_ABC_abc_FP32 == id);
    ush_assert(1 == success);

    pthread_mutex_lock(&mutex);
    flag_done = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return USH_RET_OK;
}

static ush_ret_t onRcv(ush_sig_id_t sigid, const ush_pvoid_t data) {
    ush_assert(USH_SIG_ID_ABC_abc_FP32 == sigid);
    (void)data;
    pthread_mutex_lock(&mutex);
    flag_rcv = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return USH_RET_OK;

}

void test_ush_sig_reg(void) {

    ush_ret_t ret = OK;
    ush_char_t name[] = "TEST_USH_SIG_REG";

    ret = ush_pipe_create(name, USH_PP_MODE_STD, 0, 0, NULL, 0, &sPipe);
    ush_assert(OK == ret);


    ush_sig_reg_conf_t conf0 ={USH_SIG_ID_ABC_abc_FP32, onReg, NULL};
    ret = ush_sig_reg(sPipe, &conf0);
    ush_assert(OK == ret);
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex); // wait the cb 'done' signal
    pthread_mutex_unlock(&mutex);
    ush_assert(1 == flag_done);   // coredump if failed.
    flag_done = 0;

    conf0.rcv = onRcv; // use receive callback
    ret = ush_sig_reg(sPipe, &conf0);
    ush_assert(OK == ret);
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex); // wait the cb 'rcv' signal
    pthread_mutex_unlock(&mutex);
    // ush_assert(1 == flag_rcv);   // coredump if failed.
    // flag_rcv = 0;

    ret = ush_sig_reg(0, &conf0);
    ush_assert(OK != ret);

    ret = ush_sig_reg(sPipe, NULL);
    ush_assert(OK != ret);

    ret = ush_sig_reg(0, NULL);
    ush_assert(OK != ret);

    ush_sig_reg_conf_t conf1 ={USH_SIG_ID_MAX, onReg, NULL};
    ret = ush_sig_reg(sPipe, &conf1);
    ush_assert(OK != ret);

    ush_sig_reg_conf_t conf2 ={USH_SIG_ID_INVALID, onReg, NULL};
    ret = ush_sig_reg(sPipe, &conf2);
    ush_assert(OK != ret);

    ush_sig_reg_conf_t conf3 ={USH_SIG_ID_ABC_abc_FP32, NULL, NULL};
    ret = ush_sig_reg(sPipe, &conf3);
    ush_assert(OK == ret);
}
