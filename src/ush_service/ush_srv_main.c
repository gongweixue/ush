#include "ush_srv_touch.h"


int main () {

    ush_ret_t res = ush_srv_thread_start_touch_daemon();
    if (USH_RET_OK != res) {
    }

    while (1) {
        sleep(2);
    }

    return 0;
}
