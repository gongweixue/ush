#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_comm_touch.h"
#include "ush_log.h"

#include "ushd_touch_thread.h"
#include "ushd_touch.h"

typedef struct ushd_touch_thread {
    ush_bool_t       tidFlg; // tid initial flag
    pthread_t        tid;    // thread id for the listener
    ushd_touch_t     touch;     // mq handle
} * ushd_touch_thread_t;


ushd_touch_thread_t touch_thread = NULL;

ushd_touch_thread_t
ushd_touch_thread_singleton() {
    if (!touch_thread) { // null test without lock
        ushd_touch_thread_cs_enter();

        if (!touch_thread) {
            // malloc touch_thread to touch_thread
            touch_thread =
                (ushd_touch_thread_t)malloc(sizeof(struct ushd_touch_thread));

            touch_thread->tidFlg = 0;
            touch_thread->tid    = 0xFFFFFFFF; // maybe a valid value
        }

        ushd_touch_thread_cs_exit();
    }
    return touch_thread;
}


pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;

ush_ret_t
ushd_touch_thread_cs_enter() {
    return !pthread_mutex_lock(&cs_mutex) ? USH_RET_OK : USH_RET_FAILED;
}
ush_ret_t
ushd_touch_thread_cs_exit() {
    return !pthread_mutex_unlock(&cs_mutex) ? USH_RET_OK : USH_RET_FAILED;
}

ush_ret_t
ushd_touch_thread_set_id(pthread_t tid) {
    ushd_touch_thread_t touch_thread = ushd_touch_thread_singleton();
    if (!touch_thread || touch_thread->tidFlg) {
        ushd_log(LOG_LVL_FATAL, "can not set tid to NULL or alreay set id");
        return USH_RET_FAILED;
    }

    ushd_touch_thread_cs_enter();

    touch_thread->tid    = tid;
    touch_thread->tidFlg = 1;

    ushd_touch_thread_cs_exit();

    return USH_RET_OK;
}


void *
ushd_touch_thread_entry(void *arg) {
    ushd_touch_thread_t touch_thread = ushd_touch_thread_singleton();
    if (!touch_thread) {
        ushd_log(LOG_LVL_FATAL, "singleton touch thread NULL");
        pthread_exit(NULL);
    }

    // struct mq_attr qAttr;
    // memset(&qAttr, 0, sizeof(qAttr));
    // qAttr.mq_maxmsg  = USH_COMM_TOUCH_Q_MSG_MAX_CNT
    // qAttr.mq_msgsize = USH_COMM_TOUCH_Q_MSG_MAX_LEN;
    // mqd_t mq = mq_open(USH_COMM_TOUCH_Q_PATH,
    //                    O_RDONLY | O_CREAT, // read end
    //                    S_IRWXU | S_IRWXG, // 0770
    //                    &qAttr);
    // if (-1 == mq) {
    //     return USH_RET_FAILED;
    // }

    // touch_thread->mq = mq;

    while (1) {
        // ush_char_t  msg[USH_COMM_TOUCH_Q_MSG_MAX_LEN];
        // ushd_log(LOG_LVL_INFO, "touch receiving...");
        // ush_ssize_t rcv_sz = mq_receive(mq, buff, sizeof(buff), NULL);

    //     if (-1 == rcv_sz) {
    //         ushd_log(LOG_LVL_ERROR, "ERROR rcv_sz");
    //         continue;
    //     }
    //     touch_dispatch(buff);
    }

    return 0;
}


ush_ret_t
ushd_touch_thread_start() {
    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, ushd_touch_thread_entry, NULL)) {
        ushd_log(LOG_LVL_FATAL, "create touch daemon thread: failed.");
        return USH_RET_FAILED;
    }

    if (0 != pthread_detach(tid)) {
        ushd_log(LOG_LVL_ERROR, "detach touch daemon thread: failed.");
        return USH_RET_FAILED;
    }


    if (USH_RET_OK != ushd_touch_thread_set_id(tid)) {
        ushd_log(LOG_LVL_FATAL, "touch thread set id failed");
        pthread_cancel(tid);
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}
