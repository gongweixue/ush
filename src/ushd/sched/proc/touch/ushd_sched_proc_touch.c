#include "ush_comm.h"
#include "ush_log.h"
#include "ushd_sched_proc_touch.h"
#include "ushd_sched_proc_touch_hello.h"
#include "ushd_sched_proc_touch_sig.h"

void ushd_sched_proc_touch(const ush_pvoid_t ptr) {

    const ush_touch_msg_description *pDest =
        (const ush_touch_msg_description *)ptr;

    switch (pDest->catalog) {
    case USH_COMM_TOUCH_MSG_CATALOG_HELLO:
        ushd_sched_proc_touch_hello(ptr);
        break;
    case USH_COMM_TOUCH_MSG_CATALOG_SIG:
        ushd_sched_proc_touch_sig(ptr);
        break;
    default:
        ushd_log(LOG_LVL_ERROR, "wrong catalog #%d", pDest->catalog);
        break;
    }

}
