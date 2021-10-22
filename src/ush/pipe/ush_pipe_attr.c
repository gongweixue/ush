
#include "ush_assert.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_type_pub.h"


ush_ret_t
ush_pipe_query(ush_pipe_t pipe, USH_PIPE_ATTR attr, ush_pvoid_t ptr) {
    (void)pipe;
    (void)attr;
    (void)ptr;
    ush_assert(0);
    return USH_RET_OK;
}

ush_ret_t
ush_pipe_update(ush_pipe_t pipe, USH_PIPE_ATTR attr, const ush_pvoid_t ptr) {
    (void)pipe;
    (void)attr;
    (void)ptr;
    ush_assert(0);
    return USH_RET_OK;
}
