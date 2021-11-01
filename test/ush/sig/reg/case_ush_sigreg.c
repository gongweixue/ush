#include "test-common.h"
#include "ush_pipe_pub.h"
#include "ush_sig_pub.h"
#include "ush_sig_id.h"
#include "ush_string.h"

#include "pthread.h"

////////////////////////////////
static int flg = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
static ush_ret_t onReg_normal(ush_pipe_t pp, ush_sig_id_t id, ush_bool_t succ) {
    ush_assert(pp == pp);
    ush_assert(USH_SIG_ID_ABC_abc_FP32 == id);
    ush_assert(USH_TRUE == succ);

    pthread_mutex_lock(&mutex);

    flg = 1;

    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&mutex);

    return USH_RET_OK;
}
static void case_normal(void) {
    ush_ret_t ret = OK;
    ush_pipe_t pp = USH_INVALID_PIPE; // magic num
    ush_char_t name[] = "TEST_SIGREG_normal";

    ret = ush_pipe_create(name, 0, 0, &pp);
    ush_assert(OK == ret);

    pthread_mutex_lock(&mutex);

    ush_sigreg_conf_t conf ={USH_SIG_ID_ABC_abc_FP32, onReg_normal, NULL};
    ret = ush_sigreg(pp, &conf);
    ush_assert(OK == ret);

    pthread_cond_wait(&cond, &mutex); // wait the cb 'done' signal

    ush_assert(1 == flg);   // coredump if failed.

    pthread_mutex_unlock(&mutex);
}

/////////////////////////////
static void case_wrong_param(void) {
    // prepare pipe
    ush_ret_t ret = OK;
    ush_pipe_t pp = USH_INVALID_PIPE;
    ush_char_t name[] = "TEST_sigreg_param";
    ret = ush_pipe_create(name, 0, 0, &pp);
    ush_assert(OK == ret);

    ush_sigreg_conf_t conf ={USH_SIG_ID_ABC_abc_FP32, onReg_normal, NULL};

    // invalid pipe
    ret = ush_sigreg(USH_INVALID_PIPE, &conf);
    ush_assert(OK != ret);

    // NULL conf
    ret = ush_sigreg(pp, NULL);
    ush_assert(OK != ret);

    // invalid sigid
    ush_sigreg_conf_t conf_invalid ={USH_SIG_ID_MAX, onReg_normal, NULL};
    ret = ush_sigreg(pp, &conf_invalid);
    ush_assert(OK != ret);

    // invalid sigid
    conf_invalid.sigid = USH_SIG_ID_INVALID;
    ret = ush_sigreg(pp, &conf_invalid);
    ush_assert(OK != ret);
}

///////////////////////
static void case_no_cb(void) {
    ush_ret_t ret = OK;
    ush_pipe_t pp = USH_INVALID_PIPE;
    ush_char_t name[] = "TEST_SIGREG_NO_CB";
    ret = ush_pipe_create(name, 0, 0, &pp);
    ush_assert(OK == ret);

    ush_sigreg_conf_t conf ={USH_SIG_ID_ABC_abc_FP32, NULL, NULL};
    ret = ush_sigreg(pp, &conf);
    ush_assert(OK == ret);
}



static void test_ush_sigreg(void) {
    case_normal();
    case_wrong_param();
    case_no_cb();
}



int main(void) {
    test_ush_sigreg();
    return 0;
}
