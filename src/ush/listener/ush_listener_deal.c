
#include "ush_assert.h"
#include "ush_log.h"

#include "ush_comm_howareyou_msg.h"
#include "ush_listener_deal.h"
#include "ush_sync.h"

ush_ret_t
ush_listener_deal(ush_listener_msg_desc_t *desc) {
    ush_assert(desc);
    if (USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU == desc->catalog) {
        ush_comm_howareyou_msg *howareyou = (ush_comm_howareyou_msg *)desc;

        ush_connect_ident ident  = howareyou->ident;
        ush_sync_hello_ack_t ack = (ush_sync_hello_ack_t)(howareyou->sync);

        ush_log(LOG_LVL_INFO, "signal the ack for hello");
        ush_sync_hello_ack_signal(ack, ident); // no matter ok nor not
    }

    return USH_RET_OK;
}
