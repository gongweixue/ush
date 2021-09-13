#include "unistd.h"
#include "stdio.h"
#include "ush_srv_touch.h"
#include "ush_log.h"


int main () {

    ush_ret_t ret = ush_srv_touch_start();
    if (USH_RET_OK != ret) {
        ush_log(USH_LOG_LVL_FATAL, "touch-thread starts failed");
        return ret;
    }


    printf("main thread running...\n");
    while (1) {
    }

    return 0;
}
