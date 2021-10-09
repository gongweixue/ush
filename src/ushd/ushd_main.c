
#include "ush_log.h"

#include "sched/ushd_sched_thread.h"
#include "touch/ushd_touch_thread.h"

int main () {

    ush_ret_t ret = USH_RET_OK;

    ushd_log(LOG_LVL_INFO, "starting sched of ushd");
    ret = ushd_sched_thread_start();
    if (USH_RET_OK != ret) {
        ushd_log(LOG_LVL_FATAL, "sched of ushd starts failed");
        return ret;
    }

    ushd_log(LOG_LVL_INFO, "starting touch of ushd");
    ret  = ushd_touch_thread_start();
    if (USH_RET_OK != ret) {
        ushd_log(LOG_LVL_FATAL, "touch of ushd starts failed");
        return ret;
    }


    ushd_log(LOG_LVL_INFO, "ushd main thread running...");
    while (1) {
    }

    return 0;
}
