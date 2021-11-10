
#include "ush_comm_desc.h"
#include "realm/ush_comm_realm.h"
#include "ush_log.h"
#include "ushd_sched_proc_realm.h"
#include "ushd_sched_proc_realm_sig.h"

void ushd_sched_proc_realm(const ush_pvoid_t ptr) {

    const ush_comm_realm_msg_d *pDest = (const ush_comm_realm_msg_d *)ptr;

    switch (pDest->catalog) {
    case USH_COMM_REALM_MSG_CATALOG_SIG:
        ushd_log(LOG_LVL_INFO, "proc sig");
        ushd_sched_proc_realm_sig(ptr);
        break;

    default:
        ushd_log(LOG_LVL_ERROR, "wrong catalog #%d", pDest->catalog);
        break;
    }

}
