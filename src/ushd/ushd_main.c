
#include "ush_log.h"

#include "ushd_dispatch.h"
#include "ushd_touch_thread.h"

int main () {

    ush_ret_t ret = USH_RET_OK;

    ushd_log(LOG_LVL_INFO, "starting dispatch of ushd");
    ret = ushd_dispatch_start();
    if (USH_RET_OK != ret) {
        ushd_log(LOG_LVL_FATAL, "dispatch of ushd starts failed");
        return ret;
    }

    ushd_log(LOG_LVL_INFO, "starting touch of ushd");
    ret  = ushd_touch_thread_start();
    if (USH_RET_OK != ret) {
        ushd_log(LOG_LVL_FATAL, "touch of ushd starts failed");
        return ret;
    }


    ushd_log(LOG_LVL_INFO, "ushd main thread running...\n");
    while (1) {
    }

    return 0;
}
