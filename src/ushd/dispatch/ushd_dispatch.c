#include "ushd_dispatch.h"

ush_ret_t
ushd_dispatch_start() {
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
