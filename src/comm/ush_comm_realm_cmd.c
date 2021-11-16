
#include "stdlib.h"

#include "ush_log.h"

#include "ush_comm_realm.h"
#include "ush_comm_realm_cmd.h"


typedef struct comm_realm_cmd_s {
    ush_comm_realm_msg_d         desc;
    USH_COMM_REALM_CMD_ID        cmdid;
} USH_COMM_MSG_PACKED  * ush_comm_realm_cmd_t;

ush_ret_t
ush_comm_realm_cmd_create(ush_comm_realm_cmd_t *pMsg, USH_COMM_REALM_CMD_ID id){
    if (!pMsg) {
        ush_log(LOG_LVL_ERROR, "ptr NULL");
        return USH_RET_WRONG_PARAM;
    }

    *pMsg =(ush_comm_realm_cmd_t)malloc(sizeof(struct comm_realm_cmd_s));
    if (!*pMsg) {
        ush_log(LOG_LVL_ERROR, "out of mem");
        return USH_RET_OUT_OF_MEM;
    }

    (*pMsg)->desc.desc.port = USH_COMM_PORT_REALM;
    (*pMsg)->desc.catalog   = USH_COMM_REALM_MSG_CATALOG_CMD;
    (*pMsg)->cmdid          = id;

    return USH_RET_OK;
}

USH_COMM_REALM_CMD_ID
ush_comm_realm_cmd_id_of(const ush_comm_realm_cmd_t msg) {
    if (!msg) {
        ush_log(LOG_LVL_ERROR, "wrong parameter: NULL");
        return USH_COMM_REALM_CMD_ID_INVALID;
    }
    return msg->cmdid;
}

ush_ret_t
ush_comm_realm_cmd_destroy(ush_comm_realm_cmd_t *pMsg) {
    if (!pMsg) {
        ush_log(LOG_LVL_INFO, "msg ptr null");
        return USH_RET_OK;
    }

    ush_comm_realm_cmd_t msg = *pMsg;
    if (!msg) {
        ush_log(LOG_LVL_INFO, "msg ptr null");
        return USH_RET_OK;
    }

    free(msg);
    *pMsg = NULL;

    return USH_RET_OK;
}

ush_size_t
ush_comm_realm_cmd_sizeof(void) {
    return sizeof(struct comm_realm_cmd_s);
}

void
ush_comm_realm_cmd_testpoint(const ush_comm_realm_cmd_t msg) {
    (void)msg;
}
