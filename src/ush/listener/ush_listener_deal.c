
#include "ush_assert.h"
#include "ush_log.h"

#include "ush_comm.h"
#include "ush_listener_deal.h"
#include "ush_listener_routine.h"

ush_ret_t
ush_listener_deal(ush_listener_msg_description *desc) {
    ush_assert(desc);
    if (USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU == desc->catalog) {
        ush_comm_howareyou_msg_t msg = (ush_comm_howareyou_msg_t)desc;
        ush_comm_howareyou_msg_testpoint(msg);
        ush_listener_routine_howareyou(msg);
    }

    return USH_RET_OK;
}
