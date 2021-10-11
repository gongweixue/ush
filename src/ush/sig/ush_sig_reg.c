#include "ush_def_pub.h"
#include "ush_connect.h"
#include "ush_log.h"
#include "ush_sig_pub.h"

#include "ush_comm_sig_msg.h"

ush_ret_t
ush_sig_reg(ush_pipe_t pipe, const ush_sig_reg_conf_t *pconf) {
    if (0 == pipe || NULL == pconf) {
        ush_log(LOG_LVL_ERROR, "passing 0 as the param");
        return USH_RET_WRONG_PARAM;
    }

    ush_connect_t conn = (ush_connect_t)pipe;
    ush_s32_t cert = USH_INVALID_CERT_VALUE_DEFAULT;
    if (USH_RET_OK != ush_connect_get_cert(conn, &cert)) {
        ush_log(LOG_LVL_ERROR, "invalid pipe");
        return USH_RET_WRONG_SEQ;
    }

    ush_s32_t idx = 0;
    if (USH_RET_OK != ush_connect_get_remote_idx(conn, &idx)) {
        ush_log(LOG_LVL_ERROR, "remote conn idx getting failed");
        return USH_RET_WRONG_SEQ;
    }

    ush_touch_t touch = NULL;
    if (USH_RET_OK != ush_connect_get_touch(conn, &touch)) {
        ush_log(LOG_LVL_ERROR, "conn's touch getting failed");
        return USH_RET_FAILED;
    }

    if (!ush_sig_id_valid(pconf->sigid)) {
        ush_log(LOG_LVL_ERROR, "wrong sig id");
        return USH_RET_WRONG_PARAM;
    }

    ush_comm_sig_msg_reg_t msg = {
        {{USH_COMM_TOUCH_MSG_CATALOG_SIG}, USH_COMM_TOUCH_SIG_INTENT_REG},
        idx, cert, pconf->sigid, pconf->done, pconf->rcv
    };

    return ush_touch_send(touch, (const ush_char_t*)(&msg), sizeof(msg));
}
