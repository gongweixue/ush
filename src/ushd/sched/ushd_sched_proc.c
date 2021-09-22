

#include "ush_assert.h"
#include "ush_comm_hello_msg.h"
#include "ush_comm_touch.h"
#include "ush_log.h"

#include "ushd_sched_proc.h"

typedef void (*proc_func_t)(const ush_vptr_t ptr);

typedef struct {
    // USH_COMM_TOUCH_MSG_CATALOG catelog;
    proc_func_t                func;
} proc_function;

void proc_func_hello(const ush_vptr_t msg);

// pair by STL? so the catalog could be index to the function directly.
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

    const ush_char_t *pName   = ush_comm_hello_msg_get_name(msg);
    const ush_vptr_t  ackSync = ush_comm_hello_msg_get_ack(msg);
    ush_s32_t         cert    = ush_comm_hello_msg_get_cert(msg);
    // get all data
    // build a list to store connection
    // start a thread as publisher
    // publisher open the listener queue and send howareyou
}
