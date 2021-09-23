#include "pthread.h"
#include "stdlib.h"

#include "ush_comm_hello_msg.h"
#include "ush_comm_touch.h"

#include "ush_log.h"
#include "ush_time.h"

#include "ushd_conn_record_tbl.h"
#include "ushd_sched_fifo.h"
#include "ushd_sched_proc.h"
#include "ushd_sched_thread.h"

void *
ushd_sched_thread_entry(void *arg) {
    ushd_log(LOG_LVL_INFO, "starting the touch thread entry");

    ushd_log(LOG_LVL_DETAIL, "sched fifo init");
    if (USH_RET_OK != ushd_sched_fifo_init()) {
        ushd_log(LOG_LVL_FATAL, "sched fifo init failed.");
        goto TERMINATE;
    }

    if (USH_RET_OK != ushd_conn_table_init()) {
        ushd_log(LOG_LVL_FATAL, "conn-table init failed.");
        goto TERMINATE;
    }

    while(1) {
        ush_char_t *pbuf = ushd_sched_fifo_retain(USHD_SCHED_FIFO_FULL);
        if (NULL == pbuf) {
            ushd_log(LOG_LVL_ERROR, "sched_fifo full buffer retain failed");
            continue;
        }

        ushd_log(LOG_LVL_INFO, "dispatch the buffer from sched-full-queue");
        ushd_sched_proc(pbuf);

        ushd_sched_fifo_release(pbuf, USHD_SCHED_FIFO_EMPTY);
    }

TERMINATE:
    exit(0);
}


ush_ret_t
ushd_sched_thread_start() {
    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, ushd_sched_thread_entry, NULL)) {
        ushd_log(LOG_LVL_FATAL, "create sched daemon thread: failed.");
        return USH_RET_FAILED;
    }

    ushd_log(LOG_LVL_DETAIL, "ushd_sched_thread start with tid %lu", tid);

    if (0 != pthread_detach(tid)) {
        ushd_log(LOG_LVL_ERROR, "detach sched daemon thread: failed.");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}
