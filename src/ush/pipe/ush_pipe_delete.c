
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_type_pub.h"

ush_ret_t ush_pipe_delete(ush_pipe_t pipe) {
    if (USH_INVALID_PIPE == pipe) {
        ush_log(LOG_LVL_ERROR, "invalid pipe");
        return USH_RET_WRONG_PARAM;
    }
    ush_connect_t conn = pipe;
    if (!ush_connect_valid(conn)) {
        ush_log(LOG_LVL_ERROR, "invalid pipe");
        return USH_RET_WRONG_PARAM;
    }

    ush_connect_destroy();


    return USH_RET_FAILED;
}
