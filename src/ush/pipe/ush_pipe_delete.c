
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_type_pub.h"

ush_ret_t ush_pipe_delete(ush_pipe_t pipe) {
    if (USH_INVALID_PIPE == pipe) {
        ush_log(LOG_LVL_ERROR, "invalid pipe");
        return USH_RET_WRONG_PARAM;
    }
    ush_connect_t conn = (ush_connect_t)pipe;
    if (!ush_connect_valid(conn)) {
        ush_log(LOG_LVL_ERROR, "invalid pipe");
        return USH_RET_WRONG_PARAM;
    }

    ush_cert_t cert = USH_INVALID_CERT_VALUE;
    if (USH_RET_OK != ush_connect_get_cert(conn, &cert)) {
        ush_log(LOG_LVL_ERROR, "invalid cert");
        return USH_RET_WRONG_SEQ;
    }

    ush_connidx_t idx = 0;
    if (USH_RET_OK != ush_connect_get_connidx(conn, &idx)) {
        ush_log(LOG_LVL_ERROR, "remote conn idx getting failed");
        return USH_RET_WRONG_SEQ;
    }

    if (USH_RET_OK != ush_connect_goodbye(conn)) {
        ush_log(LOG_LVL_ERROR, "bye with service failed");
        return USH_RET_FAILED;
    }

    // ush_connect_destroy(&conn);


    return USH_RET_OK;
}
