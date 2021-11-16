#include "ush_assert.h"
#include "ush_log.h"

#include "conn/ushd_conn_tbl.h"
#include "ushd_sched_proc_tch_goodbye.h"
#include "ush_comm_tch_goodbye.h"

void ushd_sched_proc_tch_goodbye(const ush_pvoid_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "msg is NULL!!!");
        return;
    }

    const ush_comm_tch_goodbye_t goodbye = (const ush_comm_tch_goodbye_t)msg;
    ush_connidx_t connidx = ush_comm_tch_goodbye_connidx_of(goodbye);
    if (USH_FALSE == ushd_conn_tbl_connidx_check(connidx)) {
        ushd_log(LOG_LVL_ERROR, "invalid connidx num: %d", connidx);
        return;
    }

    if (USH_FALSE == ushd_conn_tbl_get_active_flg(connidx)) {
        ushd_log(LOG_LVL_ERROR, "connidx: %d is not actived", connidx);
        return;
    }

    ush_cert_t cert = ush_comm_tch_goodbye_cert_of(goodbye);
    if (ushd_conn_tbl_get_cert(connidx) != cert) {
        ushd_log(LOG_LVL_ERROR, "wrong cert %d of connidx %d", cert, connidx);
        return;
    }

    // get a dist ptr before remove it from tbl
    ushd_dist_thread_t dist = ushd_conn_tbl_get_dist(connidx);

    if (USH_RET_OK != ushd_conn_tbl_remove(connidx)) {
        ushd_log(LOG_LVL_ERROR, "connidx: %d can not be deactive", connidx);
    }

    // kill the dist, and the record in tbl is all NULL
    ushd_dist_thread_request_stop(&dist);

    return;
}
