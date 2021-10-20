
#include "ush_assert.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ush_comm_desc.h"
#include "ush_lstnr_proc.h"
#include "ush_lstnr_proc_hay.h"
#include "ush_lstnr_proc_sigreg_ack.h"

ush_ret_t
ush_lstnr_proc(ush_comm_lstnr_msg_d *msgd) {
    ush_assert(msgd);
    switch (msgd->catalog)
    {
    case USH_COMM_LSTNR_MSG_CATALOG_HAY:
        ush_lstnr_proc_hay((ush_comm_lstnr_hay_t)msgd);
        break;

    case USH_COMM_LSTNR_MSG_CATALOG_SIGREG_ACK:
        ush_lstnr_proc_sigreg_ack((ush_comm_lstnr_sigreg_ack_t)msgd);
        break;

    case USH_COMM_LSTNR_MSG_CATALOG_MAX:
    default:
        ush_log(LOG_LVL_ERROR, "wrong listener msg catalog");
        break;
    }

    return USH_RET_OK;
}
