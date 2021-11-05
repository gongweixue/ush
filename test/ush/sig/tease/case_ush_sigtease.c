#include "test-common.h"
#include "ush_pipe_pub.h"
#include "ush_sig_pub.h"
#include "ush_sig_id.h"
#include "pthread.h"

static ush_sig_val_t ref;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

static ush_ret_t onRcv_XYZ(ush_sig_id_t sigid,
                           const ush_sig_val_t val,
                           ush_u32_t rollingcounter) {
    ush_assert(USH_SIG_ID_XYZ_xyz_U64 == sigid);
    ush_assert(ref.dataU64 == val.dataU64);
    ush_assert(0 != rollingcounter);

    // trigger the main thread moving on.
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return USH_RET_OK;
}
static void case_normal(void) {
    ush_pipe_t pipe   = USH_INVALID_PIPE;
    ush_ret_t  ret    = OK;
    ush_char_t name[] = "TEST_SIGTEASE_OK";

    ret = ush_pipe_create(name, 0, 0, &pipe);
    ush_assert(OK == ret);

    ref.dataU64 = 666;

    ret = ush_sigset(pipe, USH_SIG_ID_XYZ_xyz_U64, ref);
    ush_assert(OK == ret);



    ush_sigreg_conf_t conf ={USH_SIG_ID_XYZ_xyz_U64, NULL, onRcv_XYZ};
    ret = ush_sigreg(pipe, &conf);
    ush_assert(OK == ret);

    // in case that the callback execute too early
    pthread_mutex_lock(&mutex);
    ret = ush_sigtease(pipe, USH_SIG_ID_XYZ_xyz_U64);
    ush_assert(USH_RET_OK == ret);
    pthread_cond_wait(&cond, &mutex); // wait the cb 'rcv' signal
    pthread_mutex_unlock(&mutex);

}

static void case_wrong(void) {
    // execute after the normal case.

    ush_pipe_t pipe   = USH_INVALID_PIPE;
    ush_ret_t  ret    = OK;
    ush_char_t name[] = "TEST_SIGTEASE_NOK";

    // wrong pipe
    ret = ush_sigtease(USH_INVALID_PIPE, USH_SIG_ID_XYZ_xyz_U64);
    ush_assert(OK != ret);

    // create a pipe
    ret = ush_pipe_create(name, 0, 0, &pipe);
    ush_assert(OK == ret);

    // wrong id
    ret = ush_sigtease(pipe, USH_SIG_ID_MAX);
    ush_assert(OK != ret);
    // wrong id
    ret = ush_sigtease(pipe, USHD_INVALID_CONN_IDX_VALUE);
    ush_assert(OK != ret);
}

int main(void) {
    case_normal();
    case_wrong();

    return 0;
}
