#include "ush_assert.h"
#include "ush_comm.h"
#include "ush_log.h"
#include "ushd_sched_proc_touch_sig.h"

void ushd_sched_proc_touch_sig_reg(const ush_comm_sig_msg_reg_t *msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "NULL reg msg");
        return;
    }

    (void)msg;

}

void ushd_sched_proc_touch_sig(const ush_pvoid_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "msg is NULL!!!");
        return;
    }

    const ush_comm_sig_msg_description *pDesc =
            (const ush_comm_sig_msg_description *)msg;

    switch (pDesc->intent) {
    case USH_COMM_TOUCH_SIG_INTENT_REG:
        ushd_sched_proc_touch_sig_reg((ush_comm_sig_msg_reg_t *)msg);
        break;
    default:
        ushd_log(LOG_LVL_FATAL, "wrong intent #%d", pDesc->intent);
        break;
    }
}
