
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_sig_pub.h"

#include "ush_comm_desc.h"
#include "tch/ush_comm_tch.h"
#include "tch/sig/ush_comm_tch_sig.h"
#include "tch/sig/ush_comm_tch_sig_reg.h"

ush_ret_t
ush_sig_reg(ush_pipe_t pipe, const ush_sig_reg_conf_t *pconf) {
    if (0 == pipe || NULL == pconf) {
        ush_log(LOG_LVL_ERROR, "passing 0 as the param");
        return USH_RET_WRONG_PARAM;
    }

    ush_connect_t conn = (ush_connect_t)pipe;
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

    ush_tch_t touch = NULL;
    if (USH_RET_OK != ush_connect_get_tch(conn, &touch)) {
        ush_log(LOG_LVL_ERROR, "conn's touch getting failed");
        return USH_RET_FAILED;
    }

    if (!ush_sig_id_check(pconf->sigid)) {
        ush_log(LOG_LVL_ERROR, "wrong sigid id");
        return USH_RET_WRONG_PARAM;
    }

    ush_comm_tch_sig_reg_t msg = NULL;
    ush_ret_t ret = ush_comm_tch_sig_reg_create(&msg,
                                                idx,
                                                cert,
                                                pconf->sigid,
                                                pconf->done,
                                                pconf->rcv,
                                                pipe);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sigid reg msg create failed");
        return ret;
    }

    ret = ush_tch_send(touch, (const ush_char_t*)msg,
                       ush_comm_tch_sig_reg_sizeof(),
                       USH_COMM_TCH_SEND_PRIO_SIG_REG);

    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sent sigid reg msg failed");
    }
    ush_comm_tch_sig_reg_destroy(&msg); // destroy in any case

    return ret;
}
