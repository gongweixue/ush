
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_comm_touch.h"
#include "ush_def_pub.h"
#include "ush_log.h"

#include "sched/ushd_sched_fifo.h"
#include "ushd_touch_thread.h"
#include "ushd_touch.h"

typedef struct ushd_touch_thread {
    ush_bool_t       tidFlg; // tid initial flag
    pthread_t        tid;    // thread id for the listener
    ushd_touch_t     touch;     // mq handle
} * ushd_touch_thread_t;


static ushd_touch_thread_t s_touch_thread = NULL;


static ush_ret_t ushd_touch_thread_cs_enter();
static ush_ret_t ushd_touch_thread_cs_exit();

static ushd_touch_thread_t touch_thread_create();

ushd_touch_thread_t
ushd_touch_thread_singleton() {
    if (!s_touch_thread) { // null test without lock
        ushd_touch_thread_cs_enter();
        // ushd_touch_thread_t tmp;
        if (!s_touch_thread) {
            ushd_log(LOG_LVL_INFO, "touch thread create first time");
            s_touch_thread = touch_thread_create();
        }
        ushd_touch_thread_cs_exit();
    }
    return s_touch_thread;
}

ush_ret_t
ushd_touch_thread_set_id(pthread_t tid) {
    ushd_touch_thread_t touch_thread = ushd_touch_thread_singleton();
    ushd_log(LOG_LVL_DETAIL, "get thread entity singleton %p", touch_thread);
    if (!touch_thread || touch_thread->tidFlg) {
        ushd_log(LOG_LVL_FATAL, "can not set tid to NULL or alreay set id");
        return USH_RET_FAILED;
    }

    ushd_touch_thread_cs_enter();

    ushd_log(LOG_LVL_DETAIL, "set tid 0x%08lx and tid flag", tid);
    touch_thread->tid    = tid;
    touch_thread->tidFlg = 1;

    ushd_touch_thread_cs_exit();

    return USH_RET_OK;
}


void *
ushd_touch_thread_entry(void *arg) {
    ushd_log(LOG_LVL_DETAIL, "starting the touch thread entry");
    ushd_touch_thread_t touch_thread = ushd_touch_thread_singleton();
    if (!touch_thread) {
        ushd_log(LOG_LVL_FATAL, "singleton touch thread NULL");
        pthread_exit(NULL);
        goto TERMINATE;
    }

    if (USH_RET_OK != ushd_touch_thread_set_id(pthread_self())) {
        ushd_log(LOG_LVL_FATAL, "touch thread set id failed");
        goto TERMINATE;
    }
    ushd_log(LOG_LVL_DETAIL, "set touch thread tid, 0x%08lx", pthread_self());

    ushd_touch_thread_cs_enter();
    if (USH_RET_OK != ushd_touch_open(touch_thread->touch)) {
        goto TERMINATE;
    }
    ushd_touch_thread_cs_exit();
    ushd_log(LOG_LVL_DETAIL, "touch been opened %p", touch_thread->touch);

    while (1) {
        ushd_log(LOG_LVL_INFO, "touch forward to receiving new msg...");

        static ush_char_t buf[USH_COMM_TOUCH_Q_MSG_MAX_LEN];

        ushd_log(LOG_LVL_INFO, "receive from touch...");
        ush_ret_t res = USH_RET_OK;
        res = ushd_touch_receive(touch_thread->touch, buf, sizeof(buf));
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
ushd_touch_thread_start() {
    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, ushd_touch_thread_entry, NULL)) {
        ushd_log(LOG_LVL_FATAL, "create touch daemon thread: failed.");
        return USH_RET_FAILED;
    }

    ushd_log(LOG_LVL_DETAIL, "ushd_touch_thread start with tid %lu", tid);

    if (0 != pthread_detach(tid)) {
        ushd_log(LOG_LVL_ERROR, "detach touch daemon thread: failed.");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}

ushd_touch_thread_t
touch_thread_create() {
    ushd_touch_thread_t newMem =
        (ushd_touch_thread_t)malloc(sizeof(struct ushd_touch_thread));

    if (newMem) {
        if (USH_RET_OK !=  ushd_touch_create(&(newMem->touch))) {
            ushd_log(LOG_LVL_FATAL, "create ushd_touch failed");
            free(newMem);
            return NULL;
        }
        ushd_log(LOG_LVL_INFO, "ushd_touch created, addr %p", newMem->touch);

        newMem->tidFlg = 0;
        newMem->tid    = USH_INVALID_TID; // maybe a valid value
    }

    ushd_log(LOG_LVL_DETAIL, "touch_thread singleton init, %p", newMem);
    return newMem;
}



/////////////////////// critical section, just for touch thread singleton
pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;
ush_ret_t
ushd_touch_thread_cs_enter() {
    ushd_log(LOG_LVL_DETAIL, "enter cs of touch thread entity");
    return !pthread_mutex_lock(&cs_mutex) ? USH_RET_OK : USH_RET_FAILED;
}
ush_ret_t
ushd_touch_thread_cs_exit() {
    ushd_log(LOG_LVL_DETAIL, "exit cs of touch thread entity");
    return !pthread_mutex_unlock(&cs_mutex) ? USH_RET_OK : USH_RET_FAILED;
}
