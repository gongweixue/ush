
#include "ush_comm_lstnr_hay.h"

#include "dist/ushd_dist_thread.h"
#include "ushd_dist_proc_hay.h"

void ushd_dist_proc_hay_send(ushd_dist_thread_t       thread,
                             const dist_fifo_msg_hay *msg) {
    ush_assert(thread && msg);

    // construct msg
    ush_comm_lstnr_hay_t hay = ush_comm_lstnr_hay_create(msg->sync,
                                                         msg->idx,
                                                         msg->cert,
                                                         msg->deadline);
    if (!hay) {
        ushd_log(LOG_LVL_ERROR, "lstnr_hay msg mem alloc failed.");
        return;
    }

    ushd_log(LOG_LVL_INFO, "sending hay_msg from %p ......", hay);

    ush_ret_t ret = ushd_dist_thread_send_msg(thread, hay,
                                              ush_comm_lstnr_hay_sizeof(),
                                              USH_COMM_LSTNR_SEND_PRIO_HAY);
    if (USH_RET_OK != ret) {
        ushd_log(LOG_LVL_FATAL, "send hay %p to lstnr failed.", hay);
    }
    ush_comm_lstnr_hay_destroy(&hay);
}
