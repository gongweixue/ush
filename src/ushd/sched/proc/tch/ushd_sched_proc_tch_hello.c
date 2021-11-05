#include "ush_string.h"

#include "ush_comm_desc.h"
#include "tch/ush_comm_tch_hello.h"

#include "conn/ushd_conn_tbl.h"
#include "dist/ushd_dist_thread.h"

#include "ushd_sched_proc_tch_hello.h"

void ushd_sched_proc_tch_hello(const ush_pvoid_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "msg is NULL!!!");
        return;
    }

    const ush_comm_tch_hello_t hello = (const ush_comm_tch_hello_t)msg;

    const ush_char_t *shortname_ts = ush_comm_tch_hello_name_of(hello);
    const ush_pvoid_t sync         = ush_comm_tch_hello_sync_of(hello);
    ush_cert_t        cert         = ush_comm_tch_hello_cert_of(hello);
    ush_s64_t         deadline     = ush_comm_tch_hello_deadline_of(hello);

    // create dist thread
    ush_char_t fullname[USH_COMM_CONN_FULL_NAME_LEN_MAX];
    ush_string_gen_lstnr_fullname(fullname,
                                  sizeof(fullname),
                                  shortname_ts,
                                  cert);

    // create dist thread
    ushd_dist_thread_t dist = ushd_dist_thread_create(fullname);
    if (!dist) {
        ushd_log(LOG_LVL_ERROR, "dist thread create failed.");
        return;
    }
    ushd_log(LOG_LVL_INFO, "dist thread created %p.", dist);

    // create realm thread
    ush_string_gen_realm_fullname(fullname,
                                  sizeof(fullname),
                                  shortname_ts,
                                  cert);
    ushd_realm_thread_t realm = ushd_realm_thread_create(fullname);
    if (!realm) {
        ushd_log(LOG_LVL_ERROR, "%s dist thread create failed.", fullname);
        ushd_dist_thread_stop_destroy(&dist);
        return;
    }

    // add the info to the conn table
    ush_connidx_t connidx = ushd_conn_tbl_add(shortname_ts, cert, dist, realm);
    if (USHD_INVALID_CONN_IDX_VALUE == connidx) {
        ushd_log(LOG_LVL_ERROR, "%s can not add to the table", shortname_ts);
        ushd_dist_thread_stop_destroy(&dist);
        ushd_realm_thread_stop_destroy(&realm);
        return;
    }
    ushd_log(LOG_LVL_INFO, "info added to conn table, idx %d", connidx);

    ushd_log(LOG_LVL_INFO, "starting realm thread %p", realm);
    if (USH_RET_OK != ushd_realm_thread_start(realm)) {
        ushd_log(LOG_LVL_ERROR, "%s realm thread can not start", fullname);
        ushd_conn_tbl_remove(connidx);
        ushd_dist_thread_stop_destroy(&dist);
        ushd_realm_thread_stop_destroy(&realm);
        return;
    }

    ushd_log(LOG_LVL_INFO, "starting dist thread %p", dist);
    if (USH_RET_OK != ushd_dist_thread_start(dist)) {
        ushd_log(LOG_LVL_ERROR, "%s dist thread can not start", fullname);
        ushd_conn_tbl_remove(connidx);
        ushd_dist_thread_stop_destroy(&dist);
        ushd_realm_thread_stop_destroy(&realm);
    } else {
        dist_fifo_msg_hay hay = {
            {USHD_DIST_FIFO_MSG_TYPE_HAY}, sync, connidx, cert, deadline};

        ushd_dist_fifo_t fifo = ushd_dist_thread_get_fifo(dist);
        if (!fifo) {
            ushd_log(LOG_LVL_ERROR, "%s dist fifo NULL", fullname);
        }

        ushd_log(LOG_LVL_INFO, "push data to the dist fifo");
        ushd_dist_fifo_push(fifo, (dist_fifo_msg_d*)&hay, sizeof(hay));
    }

    return; // return anyway
}
