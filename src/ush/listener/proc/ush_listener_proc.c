
#include "ush_assert.h"
#include "ush_log.h"

#include "ush_comm.h"
#include "ush_listener_proc.h"
#include "ush_listener_proc_howareyou.h"

ush_ret_t
ush_listener_proc(const ush_listener_msg_description *desc) {
    ush_assert(desc);
    switch (desc->catalog)
    {
    case USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU:
        ush_listener_proc_howareyou((const ush_comm_howareyou_msg_t)desc);
        break;

    default:
        ush_log(LOG_LVL_ERROR, "wrong listener msg catalog");
        break;
    }

    return USH_RET_OK;
}
