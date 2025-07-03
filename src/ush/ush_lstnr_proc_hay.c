#include "ush_log.h"
#include "ush_sync.h"

#include "ush_lstnr_proc_hay.h"

void
ush_lstnr_proc_hay(const ush_comm_lstnr_hay_t msg) {
        if (!msg) {
            return;
        }
        if (USH_FALSE != ush_comm_lstnr_hay_outofdate(msg)) {
            ush_log(LOG_LVL_ERROR, "hay arrived out of data");
            return;
        }

        ush_connect_sync_t sync =
            (ush_connect_sync_t)ush_comm_lstnr_hay_sync_of(msg);
        ush_connidx_t      idx  = ush_comm_lstnr_hay_connidx_of(msg);
        ush_cert_t         cert = ush_comm_lstnr_hay_cert_of(msg);


        if (USH_RET_OK != ush_connect_sync_lock(sync)) {
            ush_log(LOG_LVL_FATAL, "sync locks failed");
            return;
        }

        ush_log(LOG_LVL_INFO, "signal the sync for hello");
        ush_connect_sync_signal(sync, idx, cert);

        if (USH_RET_OK != ush_connect_sync_unlock(sync)) {
            ush_log(LOG_LVL_FATAL, "sync locks failed");
            return;
        }
}
