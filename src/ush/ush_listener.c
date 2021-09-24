
#include "errno.h"
#include "mqueue.h"
#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_comm_listener.h"
#include "ush_listener.h"
#include "ush_log.h"

typedef struct ush_listener {
    mqd_t      mq;
    pthread_t  tid;
} * ush_listener_t;


ush_ret_t
ush_listener_open_and_start(ush_listener_t *pListener) {
    ush_assert(0);
    // ush_assert(pListener);

    // *pListener = NULL;

    // ush_listener_t tmp = (ush_listener_t)malloc(sizeof(struct ush_listener));
    // if (!tmp) {
    //     ush_log(LOG_LVL_FATAL, "listener alloc failed");
    //     return USH_RET_OUT_OF_MEM;
    // }
    // ush_log(LOG_LVL_DETAIL, "allocate memory for listener %p", tmp);

    // tmp->mq = -1;
    // *pListener = tmp;

    // return USH_RET_OK;
}

ush_ret_t
ush_listener_stop_and_close(ush_listener_t *pListener) {
    ush_assert(0);
    // assert(pListener);
    // if (!(*pListener)) {
    //     ush_log(LOG_LVL_INFO, "ush_listener_t NULL to be destroy");
    //     return USH_RET_OK;
    // }

    // // close it anyway, no matter if it has been opened.
    // ush_listener_close(*pListener);

    // free(*pListener);
    // ush_log(LOG_LVL_DETAIL, "listener %p destoried", *pListener);
    // *pListener = NULL;

    // return USH_RET_OK;
}
