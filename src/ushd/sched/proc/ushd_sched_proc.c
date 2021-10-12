

#include "ush_assert.h"
#include "ush_comm.h"
#include "ush_log.h"
#include "ush_string.h"

#include "ushd_sched_proc.h"
#include "touch/ushd_sched_proc_touch.h"

void ushd_sched_proc(const ush_pvoid_t ptr) {
    ush_assert(ptr);
    if (!ptr) {
        ushd_log(LOG_LVL_ERROR, "ptr is NULL!!!");
        return;
    }

    switch (((const ush_comm_port_description *)ptr)->port) {
    case USH_COMM_PORT_TOUCH:
        ushd_sched_proc_touch(ptr);
        break;
    default:
        ushd_log(LOG_LVL_ERROR, "wrong ush_comm_port");
        break;
    }
}
