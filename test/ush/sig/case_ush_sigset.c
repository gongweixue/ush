#include "test-common.h"
#include "ush_pipe_pub.h"
#include "ush_sig_pub.h"
#include "ush_sig_id.h"
#include "ush_string.h"
#include "pthread.h"

#include "case_ush_sigset.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

static ush_ret_t onRcv_ABC(ush_sig_id_t sigid, const ush_sig_val_t val) {
    ush_assert(USH_SIG_ID_ABC_abc_FP32 == sigid);
    ush_assert(val.dataFP32 > 6.5f && val.dataFP32 < 6.7f);
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return USH_RET_OK;

}

void test_ush_sigset(void) {

    ush_pipe_t pipe = USH_INVALID_PIPE; // magic num
    ush_ret_t ret = OK;
    ush_char_t name[] = "TEST_USH_SIGSET";

    ret = ush_pipe_create(name, USH_PP_MODE_STD, 0, 0, NULL, 0, &pipe);
    ush_assert(OK == ret);

    ush_sigreg_conf_t conf ={USH_SIG_ID_ABC_abc_FP32, NULL, onRcv_ABC};
    ret = ush_sigreg(pipe, &conf);
    ush_assert(OK == ret);

    FP32 f = 6.6f;
    ret = ush_sigset(pipe, USH_SIG_ID_ABC_abc_FP32, &f);
    ush_assert(OK == ret);

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex); // wait the cb 'rcv' signal
    pthread_mutex_unlock(&mutex);
}