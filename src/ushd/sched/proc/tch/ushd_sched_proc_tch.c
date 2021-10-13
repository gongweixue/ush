#include "ush_comm_port.h"
#include "tch/ush_comm_tch.h"
#include "ush_log.h"
#include "ushd_sched_proc_tch.h"
#include "ushd_sched_proc_tch_hello.h"
#include "ushd_sched_proc_tch_sig.h"

void ushd_sched_proc_tch(const ush_pvoid_t ptr) {

    const ush_comm_tch_msg_d *pDest = (const ush_comm_tch_msg_d *)ptr;

    switch (pDest->catalog) {
    case USH_COMM_TCH_MSG_CATALOG_HELLO:
        ushd_sched_proc_tch_hello(ptr);
        break;
    case USH_COMM_TCH_MSG_CATALOG_SIG:
        ushd_sched_proc_tch_sig(ptr);
        break;
    default:
        ushd_log(LOG_LVL_ERROR, "wrong catalog #%d", pDest->catalog);
        break;
    }

}
