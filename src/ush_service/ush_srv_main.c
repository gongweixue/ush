#include "ush_srv_touch.h"


int main () {

    ush_ret_t res = ush_srv_thread_start_touch_daemon();
    if (USH_RET_OK != res) {
        // teardown all thread;
    }

    // feed dog or anything else
    while (1) {
    }

    return 0;
}
