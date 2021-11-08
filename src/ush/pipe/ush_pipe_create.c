
#include "string.h"

#include "ush_connect.h"
#include "ush_log.h"
#include "ush_pipe_pub.h"
#include "ush_type_pub.h"


ush_ret_t
ush_pipe_create(
    const ush_char_t *pName,
    ush_u32_t         flag,
    ush_u16_t         timeout,
    ush_pipe_t       *pPipe)
{
    (void)flag;

    // params valid
    if (!pName || !pPipe) {
        ush_log(LOG_LVL_FATAL, "wrong params for pipe create.");
        if (pPipe) {*pPipe = USH_INVALID_PIPE;}
        return USH_RET_WRONG_PARAM;
    }

    *pPipe = USH_INVALID_PIPE; // NULL for error return;

    // pipe name validation
    if (USH_COMM_CONN_SHORTNAME_LEN_MAX < strlen(pName) || 0 == strlen(pName)) {
        ush_log(LOG_LVL_FATAL, "len of name not valie, limited to 0-%d",
                USH_COMM_CONN_SHORTNAME_LEN_MAX);
        return USH_RET_NOT_SUPPORT;
    }

    ush_ret_t ret = USH_RET_OK;

    ush_connect_t conn = NULL;
    ret = ush_connect_create(&conn, pName);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "connect create failed");
        goto RET;
    }

    ret = ush_connect_link(conn, timeout);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_FATAL, "connect_link failed");
        ush_connect_destroy(&conn);
        goto RET;
    }

    *pPipe = (ush_u64_t)conn;
    ush_log(LOG_LVL_INFO, "set conn's ptr to handle returned");

RET:
    return ret;
}
