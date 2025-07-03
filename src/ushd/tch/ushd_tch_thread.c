
#include "pthread.h"
#include "stdlib.h"

#include "ush_comm_tch.h"
#include "ush_define.h"
#include "ush_log.h"

#include "sched/ushd_sched_fifo.h"
#include "ushd_tch_thread.h"
#include "ushd_tch.h"

typedef struct ushd_tch_record_s {
    pthread_t        tid;    // thread id for the listener
    ushd_tch_t       touch;  // mq handle
} * ushd_tch_record_t;


static ushd_tch_record_t record = NULL;

static ushd_tch_record_t
tch_thread_create(void) {
    ushd_tch_record_t newMem =
        (ushd_tch_record_t)malloc(sizeof(struct ushd_tch_record_s));

    if (!newMem) {
        ushd_log(LOG_LVL_FATAL, "create ushd_tch_record_s failed");
        return NULL;
    } else {
        if (USH_RET_OK !=  ushd_tch_create(&(newMem->touch))) {
            ushd_log(LOG_LVL_FATAL, "create ushd_tch_s failed");
            free(newMem);
            return NULL;
        }
        ushd_log(LOG_LVL_INFO, "ushd_tch_s created, addr %p", newMem->touch);

        newMem->tid    = USH_INVALID_TID; // maybe a valid value
    }

    ushd_log(LOG_LVL_INFO, "tch_thread singleton init, %p", newMem);
    return newMem;
}

static void *
ushd_tch_thread_entry(void *arg) {
    (void)arg;
    ushd_log(LOG_LVL_INFO, "starting the touch thread entry");
    record = tch_thread_create();
    if (!record) {
        ushd_log(LOG_LVL_FATAL, "singleton touch thread NULL");
        pthread_exit(NULL);
        goto TERMINATE;
    }

    record->tid = pthread_self();

    if (USH_RET_OK != ushd_tch_open(record->touch)) {
        goto TERMINATE;
    }
    ushd_log(LOG_LVL_INFO, "touch been opened %p", record->touch);

    while (1) {
        ushd_log(LOG_LVL_INFO, "touch forward to receiving new msg...");

        static ush_char_t buf[USH_COMM_TCH_Q_MSG_MAX_LEN];

        ushd_log(LOG_LVL_INFO, "receive from touch...");
        ush_ret_t res = USH_RET_OK;
        res = ushd_tch_receive(record->touch, buf, sizeof(buf));
        if (USH_RET_OK != res) {
            ushd_log(LOG_LVL_ERROR, "touch %p receive msg failed", buf);
            continue;
        }

        if (USH_COMM_PORT_TCH != ((ush_comm_d *)buf)->port) {
            ushd_log(LOG_LVL_ERROR, "msg type not support");
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
ushd_tch_thread_start(void) {
    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, ushd_tch_thread_entry, NULL)) {
        ushd_log(LOG_LVL_FATAL, "create touch daemon thread: failed.");
        return USH_RET_FAILED;
    }

    ushd_log(LOG_LVL_INFO, "ushd tch thread starts");

    if (0 != pthread_detach(tid)) {
        ushd_log(LOG_LVL_ERROR, "detach touch daemon thread: failed.");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}
