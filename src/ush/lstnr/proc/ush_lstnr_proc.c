
#include "ush_assert.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ush_comm_port.h"
#include "ush_lstnr_proc.h"
#include "ush_lstnr_proc_hay.h"

ush_ret_t
ush_lstnr_proc(const ush_comm_lstnr_msg_d *desc) {
    ush_assert(desc);
    switch (desc->catalog)
    {
    case USH_COMM_LSTNR_MSG_CATALOG_HOWAREYOU:
        ush_lstnr_proc_hay((const ush_comm_lstnr_hay_t)desc);
        break;

    default:
        ush_log(LOG_LVL_ERROR, "wrong listener msg catalog");
        break;
    }

    return USH_RET_OK;
}
