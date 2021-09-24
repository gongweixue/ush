

#include "ush_assert.h"
#include "ush_comm_hello_msg.h"
#include "ush_comm_touch.h"
#include "ush_log.h"

#include "ushd_conn_record_tbl.h"
#include "ushd_publish_fifo.h"
#include "ushd_publish_fifo_msg.h"
#include "ushd_publish_thread.h"
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
    const ush_touch_msg_desc *pDescription = (const ush_touch_msg_desc *)ptr;
    func_tbl[pDescription->catalog].func(ptr);
}

////////////////////////////////////////////////////////////////////////////////

void proc_func_hello(const ush_vptr_t msg) {
    const ush_comm_hello_msg_t hello = (const ush_comm_hello_msg_t)msg;

    const ush_char_t *name   = ush_comm_hello_msg_get_name(hello);
    const ush_vptr_t  ackSync = ush_comm_hello_msg_get_ack(hello);
    ush_s32_t         cert    = ush_comm_hello_msg_get_cert(hello);

    // create publish thread
    ushd_publish_thread_t publish = ushd_publish_thread_create(name);
    if (!publish) {
        ushd_log(LOG_LVL_ERROR, "publish thread create failed.");
        return;
    }
    ushd_log(LOG_LVL_INFO, "publish thread created %p.", publish);

    // add the info to the conn table
    ush_s32_t record_idx = ushd_conn_table_add_record(name, cert, publish);
    if (-1 == record_idx) {
        ushd_log(LOG_LVL_ERROR, "conn can not add to the table");
        return;
    }
    ushd_log(LOG_LVL_INFO, "info added to conn table, idx %d", record_idx);

    ush_u64_t ident = ushd_conn_table_get_record_ident(record_idx);

    if (USH_RET_OK == ushd_publish_thread_start(publish)) {
        publish_fifo_msg_howareyou msg = {
            {USHD_PUBLISH_FIFO_CMD_HOWAREYOU}, ackSync, ident};

        ushd_publish_fifo_t fifo = ushd_publish_thread_get_fifo(publish);
        ushd_publish_fifo_push(fifo, (publish_fifo_msg_desc*)&msg, sizeof(msg));
    }

    return; // return anyway
}
