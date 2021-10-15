
#include "ush_string.h"

#include "ush_comm_desc.h"
#include "tch/ush_comm_tch_hello.h"

#include "conn/ushd_conn_record_tbl.h"
#include "dist/ushd_dist_thread.h"

#include "ushd_sched_proc_tch_hello.h"

void ushd_sched_proc_tch_hello(const ush_pvoid_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "msg is NULL!!!");
        return;
    }

    const ush_comm_tch_hello_t hello = (const ush_comm_tch_hello_t)msg;

    const ush_char_t *name    = ush_comm_tch_hello_name_of(hello);
    const ush_pvoid_t ackSync = ush_comm_tch_hello_ack_of(hello);
    ush_s32_t         cert    = ush_comm_tch_hello_cert_of(hello);

    // create dist thread
    ush_char_t certname[USH_COMM_CONN_NAME_LEN_MAX];
    ush_string_certname(certname, sizeof(certname), name, cert);

    ushd_dist_thread_t dist = ushd_dist_thread_create(certname);
    if (!dist) {
        ushd_log(LOG_LVL_ERROR, "dist thread create failed.");
        return;
    }
    ushd_log(LOG_LVL_INFO, "dist thread created %p.", dist);

    // add the info to the conn table
    ush_s32_t record_idx = ushd_conn_table_add_record(name, cert, dist);
    if (USHD_INVALID_CONN_IDX_VALUE == record_idx) {
        ushd_log(LOG_LVL_ERROR, "conn can not add to the table");
        return;
    }
    ushd_log(LOG_LVL_INFO, "info added to conn table, idx %d", record_idx);

    ushd_log(LOG_LVL_INFO, "starting dist thread %p", dist);
    if (USH_RET_OK == ushd_dist_thread_start(dist)) {
        dist_fifo_msg_hay msg = {
            {USHD_DIST_FIFO_MSG_TYPE_HAY}, ackSync, record_idx, cert};

        ushd_dist_fifo_t fifo = ushd_dist_thread_get_fifo(dist);

        ushd_log(LOG_LVL_INFO, "push data to the dist fifo");
        ushd_dist_fifo_push(fifo, (dist_fifo_msg_d*)&msg, sizeof(msg));
    }

    return; // return anyway
}
