#include "pthread.h"
#include "stdlib.h"

#include "ush_comm_hello_msg.h"
#include "ush_comm_touch.h"

#include "ush_log.h"
#include "ush_time.h"

#include "ushd_sched_thread.h"
#include "ushd_sched_fifo.h"

void *
ushd_sched_thread_entry(void *arg) {
    ushd_log(LOG_LVL_INFO, "starting the touch thread entry");

    ushd_log(LOG_LVL_DETAIL, "sched fifo init");
    if (USH_RET_OK != ushd_sched_fifo_init()) {
        ushd_log(LOG_LVL_FATAL, "sched fifo init failed.");
        goto TERMINATE;
    }

    while(1) {
        ush_char_t *pbuf = ushd_sched_fifo_retain(USHD_SCHED_FIFO_FULL);
        if (NULL == pbuf) {
            continue;
        }

        // TODO: convert to basic struct of touch msg,
        //       and branch to the worker or create a publisher thread
        ush_touch_msg_desc *desc = (ush_touch_msg_desc*)pbuf;
        // ushd_log(LOG_LVL_INFO, "delay 500 ms, seems like it do the real work");

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


// dispatch into the internal queues for dealling with.
// static void touch_dispatch(const void *pBuf);

static void touch_dispatch(const void *pBuf) {
    const ush_comm_touch_msg_t *pMsg = (const ush_comm_touch_msg_t *)pBuf;
    // process msg
        printf("receive %ld bytes\n", rcv_sz);
        ush_impl_touch_msg_t *pTouch = (ush_impl_touch_msg_t *)buff;
        printf("%d %s\n", pTouch->id, pTouch->name);

        if (USH_COMM_PROTOCOL_TOUCH_ID_PING == pTouch->id) {
            ushd_sw_open(pTouch->name);
        }
}
