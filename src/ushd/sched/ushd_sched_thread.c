#include "ush_comm_touch.h"

#include "ushd_sched_thread.h"
#include "ushd_sched_fifo.h"

ush_ret_t
ushd_sched_thread_start() {
    // first init sched fifo for pass msg from touch thread
    ushd_sched_fifo_init(USH_SCHED_FIFO_LEN, USH_COMM_TOUCH_Q_MSG_MAX_LEN);
    return 0;
}


// dispatch into the internal queues for dealling with.
// static void touch_dispatch(const void *pBuf);

// static void touch_dispatch(const void *pBuf) {
    // const ush_comm_touch_msg_t *pMsg = (const ush_comm_touch_msg_t *)pBuf;
    // // process msg
        // printf("receive %ld bytes\n", rcv_sz);
        // ush_impl_touch_msg_t *pTouch = (ush_impl_touch_msg_t *)buff;
        // printf("%d %s\n", pTouch->id, pTouch->name);

        // if (USH_COMM_PROTOCOL_TOUCH_ID_PING == pTouch->id) {
        //     ushd_sw_open(pTouch->name);
        // }
// }
