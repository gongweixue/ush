
#include "ush_define.h"
#include "ush_comm_lstnr_cmd_stop.h"

#include "ushd_dist_proc_cmd.h"


void
ushd_dist_proc_cmd_stop_dist(ushd_dist_thread_t thread) {
    if (!thread) {
        ushd_log(LOG_LVL_ERROR, "null ptr of param");
        return;
    }

    ush_comm_lstnr_cmd_stop_t msg = ush_comm_lstnr_cmd_stop_create();
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "cmd-stop msg mem alloc failed.");
        return;
    }

    ush_ret_t ret = ushd_dist_thread_send_msg(
        thread, msg,
        ush_comm_lstnr_cmd_stop_sizeof(),
        USH_COMM_LSTNR_SEND_PRIO_STOP);

    if (USH_RET_OK != ret) {
        ushd_log(LOG_LVL_ERROR, "send stop-cmd to lstnr failed.");
    }
    ush_comm_lstnr_cmd_stop_destroy(&msg);
    return;
}
