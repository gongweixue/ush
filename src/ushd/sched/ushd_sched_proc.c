

#include "ush_assert.h"
#include "ush_comm_hello_msg.h"
#include "ush_comm_touch.h"
#include "ush_log.h"

#include "ushd_sched_proc.h"

typedef void (*proc_func_t)(const void *ptr);

typedef struct {
    // USH_COMM_TOUCH_MSG_CATALOG catelog;
    proc_func_t                func;
} proc_function;

void proc_func_hello(const void *msg);

// pair by STL? so the catalog could be index to the function directly.
static proc_function func_tbl[] = {
    {/*USH_COMM_TOUCH_MSG_CATALOG_HELLO,*/ proc_func_hello}
};

void ushd_sched_proc(const void *ptr) {
    const ush_touch_msg_desc *pDescription = (const ush_touch_msg_desc *)ptr;
    func_tbl[pDescription->catalog].func(ptr);
}

////////////////////////////////////////////////////////////////////////////////

void proc_func_hello(const void *msg) {
    const ush_comm_hello_msg_t hello = (const ush_comm_hello_msg_t)msg;
    // get all data
    // build a list to store connection
    // start a thread as publisher
    // publisher open the listener queue and send howareyou
}
