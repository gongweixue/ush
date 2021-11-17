
#include "ush_connect.h"
#include "ush_define.h"
#include "ush_log.h"
#include "ush_sig_pub.h"

#include "ush_comm_desc.h"
#include "ush_comm_realm.h"
#include "ush_comm_realm_sig.h"
#include "ush_comm_realm_sigreg.h"
#include "ush_comm_realm_sigset.h"

ush_ret_t
ush_sigreg(ush_pipe_t pipe, const ush_sigreg_conf_t *pConf) {
    if (USH_INVALID_PIPE == pipe || NULL == pConf) {
        ush_log(LOG_LVL_ERROR, "passing 0 as the param");
        return USH_RET_WRONG_PARAM;
    }

    if (pConf->count > USH_SIGREG_CONF_MAX || 0 == pConf->count) {
        ush_log(LOG_LVL_ERROR, "count of sig num wrong");
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

    ush_connidx_t connidx = 0;
    if (USH_RET_OK != ush_connect_get_connidx(conn, &connidx)) {
        ush_log(LOG_LVL_ERROR, "remote conn idx getting failed");
        return USH_RET_WRONG_SEQ;
    }

    for (ush_u32_t idx = 0; idx < pConf->count; ++idx) {
        if (!ush_sigid_check(pConf->sigid[idx])) {
            ush_log(LOG_LVL_ERROR, "wrong sigid id");
            return USH_RET_WRONG_PARAM;
        }
    }

    ush_comm_realm_sigreg_t msg = NULL;
    ush_ret_t ret = ush_comm_realm_sigreg_create(&msg,connidx,cert,pipe,pConf);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sigid reg msg create failed");
        return ret;
    }

    ret = ush_connect_send_to_realm(conn, (const ush_comm_d *)msg);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "sent sigid reg msg failed");
    }
    ush_comm_realm_sigreg_destroy(&msg); // destroy in any case

    return ret;
}
