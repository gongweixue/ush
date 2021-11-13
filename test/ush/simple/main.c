#include "stdio.h"
#include "pthread.h"
#include "assert.h"
#include "unistd.h"

#include "ush_pipe_pub.h"
#include "ush_sig_pub.h"

static ush_pipe_t sPipe = USH_INVALID_PIPE;
static int sig_counter = 0;

static ush_ret_t reg_cb_10_u16(ush_pipe_t pp, ush_sigid_t id, ush_bool_t succ) {
    assert(pp == sPipe);
    assert(USH_SIG_ID_TEST_10_U16 == id);
    assert(USH_TRUE == succ);

    printf("USH_SIG_ID_TEST_10_U16 registered successful.\n");

    return USH_RET_OK;
}

static ush_ret_t reg_cb_10_u16_del(ush_pipe_t pp, ush_sigid_t id, ush_bool_t succ) {
    assert(pp == sPipe);
    assert(USH_SIG_ID_TEST_10_U16 == id);
    assert(USH_TRUE == succ);

    printf("delete pipe\n");
    ush_pipe_delete(sPipe);
    sPipe = USH_INVALID_PIPE;

    return USH_RET_OK;
}

static ush_ret_t rcv_cb_10_u16(ush_sigid_t id, ush_sig_val_t val, ush_u32_t cntr) {
    assert(id == USH_SIG_ID_TEST_10_U16);

    printf("USH_SIG_ID_TEST_10_U16: %08d, cntr=%d\n", val.dataU16, cntr);

    if (10 <= sig_counter) {
        ush_sigreg_conf_t conf ={USH_SIG_ID_TEST_10_U16, reg_cb_10_u16_del, NULL};
        ush_sigreg(sPipe, &conf);
        printf("un-register signal USH_SIG_ID_TEST_10_U16\n");
    }
    return USH_RET_OK;
}



static void *entry(void *arg) {
    (void)arg;

    ush_pipe_create("thread0", 0, 0, &sPipe);

    ush_sigreg_conf_t conf ={USH_SIG_ID_TEST_10_U16, reg_cb_10_u16, rcv_cb_10_u16};
    ush_sigreg(sPipe, &conf);


    usleep(999 * 1000); // ready to send msg and loop back to the callback
    ush_sig_val_t val;
    val.dataU16 = 0;
    while (sig_counter < 10) {

        usleep(10 * 1000); // simulate real signal interval:100ms
        val.dataU16 ++;
        ush_sigset(sPipe, USH_SIG_ID_TEST_10_U16, val);
        sig_counter++;

    }

    sleep(1);

    return NULL;
}

int main(void) {
    pthread_t tid;
    pthread_create(&tid, NULL, entry, NULL);
    pthread_join(tid, NULL);
    return 0;
}
