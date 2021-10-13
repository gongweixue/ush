#include "ush_assert.h"
#include "ush_log.h"

#include "ush_comm_port.h"
#include "tch/ush_comm_tch.h"
#include "tch/sig/ush_comm_tch_sig.h"
#include "tch/sig/ush_comm_tch_sig_reg.h"

#include "ushd_sched_proc_tch_sig.h"

static void ushd_sched_proc_tch_sig_reg(const ush_comm_tch_sig_reg_t *msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "NULL reg msg");
        return;
    }

    (void)msg;

}

void ushd_sched_proc_tch_sig(const ush_pvoid_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "msg is NULL!!!");
        return;
    }

    const ush_comm_tch_sig_d *pDesc = (const ush_comm_tch_sig_d *)msg;

    switch (pDesc->intent) {
    case USH_COMM_TCH_SIG_INTENT_REG:
        ushd_sched_proc_tch_sig_reg((ush_comm_tch_sig_reg_t *)msg);
        break;
    default:
        ushd_log(LOG_LVL_FATAL, "wrong intent #%d", pDesc->intent);
        break;
    }
}
