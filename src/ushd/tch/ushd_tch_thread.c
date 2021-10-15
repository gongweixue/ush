
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"

#include "tch/ush_comm_tch.h"
#include "ush_log.h"

#include "sched/ushd_sched_fifo.h"
#include "ushd_tch_thread.h"
#include "ushd_tch.h"

typedef struct ushd_tch_thread {
    ush_bool_t       tidFlg; // tid initial flag
    pthread_t        tid;    // thread id for the listener
    ushd_tch_t     touch;     // mq handle
} * ushd_tch_thread_t;


static ushd_tch_thread_t s_tch_thread = NULL;


static ush_ret_t ushd_tch_thread_cs_enter();
static ush_ret_t ushd_tch_thread_cs_exit();

static ushd_tch_thread_t tch_thread_create();

ushd_tch_thread_t
ushd_tch_thread_singleton() {
    if (!s_tch_thread) { // null test without lock
        ushd_tch_thread_cs_enter();
        // ushd_tch_thread_t tmp;
        if (!s_tch_thread) {
            ushd_log(LOG_LVL_INFO, "touch thread create first time");
            s_tch_thread = tch_thread_create();
        }
        ushd_tch_thread_cs_exit();
    }
    return s_tch_thread;
}

ush_ret_t
ushd_tch_thread_set_id(pthread_t tid) {
    ushd_tch_thread_t tch_thread = ushd_tch_thread_singleton();
    ushd_log(LOG_LVL_DETAIL, "get thread entity singleton %p", tch_thread);
    if (!tch_thread || tch_thread->tidFlg) {
        ushd_log(LOG_LVL_FATAL, "can not set tid to NULL or alreay set id");
        return USH_RET_FAILED;
    }

    ushd_tch_thread_cs_enter();

    ushd_log(LOG_LVL_DETAIL, "set tid 0x%08lx and tid flag", tid);
    tch_thread->tid    = tid;
    tch_thread->tidFlg = 1;

    ushd_tch_thread_cs_exit();

    return USH_RET_OK;
}


void *
ushd_tch_thread_entry(void *arg) {
    ushd_log(LOG_LVL_DETAIL, "starting the touch thread entry");
    ushd_tch_thread_t tch_thread = ushd_tch_thread_singleton();
    if (!tch_thread) {
        ushd_log(LOG_LVL_FATAL, "singleton touch thread NULL");
        pthread_exit(NULL);
        goto TERMINATE;
    }

    if (USH_RET_OK != ushd_tch_thread_set_id(pthread_self())) {
        ushd_log(LOG_LVL_FATAL, "touch thread set id failed");
        goto TERMINATE;
    }
    ushd_log(LOG_LVL_DETAIL, "set touch thread tid, 0x%08lx", pthread_self());

    ushd_tch_thread_cs_enter();
    if (USH_RET_OK != ushd_tch_open(tch_thread->touch)) {
        goto TERMINATE;
    }
    ushd_tch_thread_cs_exit();
    ushd_log(LOG_LVL_DETAIL, "touch been opened %p", tch_thread->touch);

    while (1) {
        ushd_log(LOG_LVL_INFO, "touch forward to receiving new msg...");

        static ush_char_t buf[USH_COMM_TCH_Q_MSG_MAX_LEN];

        ushd_log(LOG_LVL_INFO, "receive from touch...");
        ush_ret_t res = USH_RET_OK;
        res = ushd_tch_receive(tch_thread->touch, buf, sizeof(buf));
        if (USH_RET_OK != res) {
            ushd_log(LOG_LVL_ERROR, "touch %p receive msg failed", buf);
            continue;
        }

        // push msg
        ushd_sched_fifo_t fifo = ushd_sched_fifo_singleton();
        if (USH_RET_OK != ushd_sched_fifo_push(fifo, buf, sizeof(buf))) {
            ushd_log(LOG_LVL_ERROR, "sched fifo push failed %p", buf);
            continue;
        }
        ushd_log(LOG_LVL_INFO, "sched fifo push ok");

        // and then next receive to go
    }

TERMINATE:
    exit(0);
}


ush_ret_t
ushd_tch_thread_start() {
    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, ushd_tch_thread_entry, NULL)) {
        ushd_log(LOG_LVL_FATAL, "create touch daemon thread: failed.");
        return USH_RET_FAILED;
    }

    ushd_log(LOG_LVL_DETAIL, "ushd_tch_thread start with tid %lu", tid);

    if (0 != pthread_detach(tid)) {
        ushd_log(LOG_LVL_ERROR, "detach touch daemon thread: failed.");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}

ushd_tch_thread_t
tch_thread_create() {
    ushd_tch_thread_t newMem =
        (ushd_tch_thread_t)malloc(sizeof(struct ushd_tch_thread));

    if (newMem) {
        if (USH_RET_OK !=  ushd_tch_create(&(newMem->touch))) {
            ushd_log(LOG_LVL_FATAL, "create ushd_tch failed");
            free(newMem);
            return NULL;
        }
        ushd_log(LOG_LVL_INFO, "ushd_tch created, addr %p", newMem->touch);

        newMem->tidFlg = 0;
        newMem->tid    = USH_INVALID_TID; // maybe a valid value
    }

    ushd_log(LOG_LVL_DETAIL, "tch_thread singleton init, %p", newMem);
    return newMem;
}



/////////////////////// critical section, just for touch thread singleton
pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;
ush_ret_t
ushd_tch_thread_cs_enter() {
    ushd_log(LOG_LVL_DETAIL, "enter cs of touch thread entity");
    return !pthread_mutex_lock(&cs_mutex) ? USH_RET_OK : USH_RET_FAILED;
}
ush_ret_t
ushd_tch_thread_cs_exit() {
    ushd_log(LOG_LVL_DETAIL, "exit cs of touch thread entity");
    return !pthread_mutex_unlock(&cs_mutex) ? USH_RET_OK : USH_RET_FAILED;
}
