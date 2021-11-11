
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_sig_pub.h"

#include "realm/sig/ush_comm_realm_sigtease.h"

ush_ret_t
ush_sigtease(ush_pipe_t pipe, ush_sig_id_t sigid) {
    if (USH_INVALID_PIPE == pipe || !ush_sig_id_check(sigid)) {
        ush_log(LOG_LVL_ERROR, "parameters not correct!");
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

    ush_comm_realm_sigtease_t msg = NULL;
    ush_ret_t ret =
        ush_comm_realm_sigtease_create(&msg, idx, cert, sigid);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sigtease_msg create failed");
        return ret;
    }

    ret = ush_connect_send_to_realm(conn, (const ush_comm_d *)msg);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sent sigid reg msg failed");
    }
    ush_comm_realm_sigtease_destroy(&msg); // destroy in any case

    return ret;
}
