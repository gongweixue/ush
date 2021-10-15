#include "pthread.h"
#include "stdlib.h"

#include "tch/ush_comm_tch.h"
#include "tch/ush_comm_tch_hello.h"

#include "ush_log.h"
#include "ush_time.h"

#include "conn/ushd_conn_tbl.h"
#include "proc/ushd_sched_proc.h"
#include "ushd_sched_fifo.h"
#include "ushd_sched_thread.h"

void *
ushd_sched_thread_entry(void *arg) {
    ushd_log(LOG_LVL_DETAIL, "starting the touch thread entry");

    ushd_log(LOG_LVL_DETAIL, "sched fifo creating...");
    ushd_sched_fifo_t sched_fifo = ushd_sched_fifo_singleton();
    if (!sched_fifo) {
        ushd_log(LOG_LVL_FATAL, "sched fifo init failed.");
        goto TERMINATE;
    }

    ushd_log(LOG_LVL_DETAIL, "conn table init");
    if (USH_RET_OK != ushd_conn_tbl_init()) {
        ushd_log(LOG_LVL_FATAL, "conn-table init failed.");
        goto TERMINATE;
    }

    ushd_log(LOG_LVL_DETAIL, "reg-list init");
    if (USH_RET_OK != ushd_conn_reglist_init()) {
        ushd_log(LOG_LVL_FATAL, "reglist init failed.");
        goto TERMINATE;
    }

    while(1) {
        ushd_log(LOG_LVL_DETAIL, "retain a full buffer");
        ush_char_t buf[USHD_SCHED_FIFO_ELEM_DATA_LEN];
        size_t cnt = ushd_sched_fifo_pop(sched_fifo, buf, sizeof(buf));
        if (0 == cnt) {
            ushd_log(LOG_LVL_ERROR, "sched_fifo pop failed.");
            continue;
        }

        ushd_log(LOG_LVL_INFO, "dispatch the buffer to proc-function");
        ushd_sched_proc(buf);
    }

TERMINATE:
    exit(0);
}


ush_ret_t
ushd_sched_thread_start() {
    pthread_t tid;
    ushd_log(LOG_LVL_INFO, "starting sched daemon thread...");
    if (0 != pthread_create(&tid, NULL, ushd_sched_thread_entry, NULL)) {
        ushd_log(LOG_LVL_FATAL, "create sched daemon thread: failed.");
        return USH_RET_FAILED;
    }

    ushd_log(LOG_LVL_DETAIL, "ushd_sched_thread start with tid %lu", tid);

    ushd_log(LOG_LVL_INFO, "detaching sched daemon thread...");
    if (0 != pthread_detach(tid)) {
        ushd_log(LOG_LVL_ERROR, "detach sched daemon thread: failed.");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}
