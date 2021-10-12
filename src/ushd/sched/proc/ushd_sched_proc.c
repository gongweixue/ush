

#include "ush_assert.h"
#include "ush_comm.h"
#include "ush_log.h"
#include "ush_string.h"

#include "ushd_sched_proc.h"
#include "tch/ushd_sched_proc_tch.h"

void ushd_sched_proc(const ush_pvoid_t ptr) {
    ush_assert(ptr);
    if (!ptr) {
        ushd_log(LOG_LVL_ERROR, "ptr is NULL!!!");
        return;
    }

    switch (((const ush_comm_port_d *)ptr)->port) {
    case USH_COMM_PORT_TCH:
        ushd_sched_proc_tch(ptr);
        break;
    default:
        ushd_log(LOG_LVL_ERROR, "wrong ush_comm_port");
        break;
    }
}
