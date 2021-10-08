

#include "ush_assert.h"
#include "ush_comm_hello_msg.h"
#include "ush_comm_touch.h"
#include "ush_log.h"
#include "ush_string.h"

#include "ushd_conn_record_tbl.h"
#include "ushd_dist_fifo.h"
#include "ushd_dist_fifo_msg.h"
#include "ushd_dist_thread.h"
#include "ushd_sched_proc.h"

typedef void (*proc_func_t)(const ush_vptr_t ptr);

typedef struct {
    // USH_COMM_TOUCH_MSG_CATALOG catelog;
    proc_func_t                func;
} proc_function;

void proc_func_hello(const ush_vptr_t msg);

// pair by STL? so catalog could be index the function directly.concurrency?
static proc_function func_tbl[] = {
    {/*USH_COMM_TOUCH_MSG_CATALOG_HELLO,*/ proc_func_hello}
};

void ushd_sched_proc(const ush_vptr_t ptr) {
    ush_assert(ptr);
    if (!ptr) {
        ushd_log(LOG_LVL_ERROR, "ptr is NULL!!!");
        return;
    }
    const ush_touch_msg_description *pDescription =
        (const ush_touch_msg_description *)ptr;

    func_tbl[pDescription->catalog].func(ptr);
}

////////////////////////////////////////////////////////////////////////////////

void proc_func_hello(const ush_vptr_t msg) {
    ush_assert(msg);
    if (!msg) {
        ushd_log(LOG_LVL_ERROR, "msg is NULL!!!");
        return;
    }

    const ush_comm_hello_msg_t hello = (const ush_comm_hello_msg_t)msg;

    const ush_char_t *name    = ush_comm_hello_msg_name_of(hello);
    const ush_vptr_t  ackSync = ush_comm_hello_msg_ack_of(hello);
    ush_s32_t         cert    = ush_comm_hello_msg_cert_of(hello);

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
    if (-1 == record_idx) {
        ushd_log(LOG_LVL_ERROR, "conn can not add to the table");
        return;
    }
    ushd_log(LOG_LVL_INFO, "info added to conn table, idx %d", record_idx);

    ushd_log(LOG_LVL_INFO, "starting dist thread %p", dist);
    if (USH_RET_OK == ushd_dist_thread_start(dist)) {
        dist_fifo_msg_howareyou msg = {
            {USHD_DIST_FIFO_CMD_HOWAREYOU}, ackSync, record_idx, cert};

        ushd_dist_fifo_t fifo = ushd_dist_thread_get_fifo(dist);

        ushd_log(LOG_LVL_INFO, "push data to the dist fifo");
        ushd_dist_fifo_push(fifo, (dist_fifo_msg_desc*)&msg, sizeof(msg));
    }

    return; // return anyway
}
