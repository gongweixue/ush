
#include "ush_log.h"

#include "ushd_touch.h"

int main () {

    ush_ret_t ret = ushd_touch_start();
    if (USH_RET_OK != ret) {
        ush_log(USH_LOG_LVL_FATAL, "touch-thread starts failed");
        return ret;
    }


    printf("ushd main thread running...\n");
    while (1) {
    }

    return 0;
}
