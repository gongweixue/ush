#include "stdlib.h"

#include "ush_log.h"
#include "ush_comm_lstnr.h"
#include "ush_comm_lstnr_cmd_stop.h"

typedef struct lstnr_cmd_stop_s {
    ush_comm_lstnr_msg_d         desc;
} USH_COMM_MSG_PACKED * ush_comm_lstnr_cmd_stop_t;

ush_comm_lstnr_cmd_stop_t
ush_comm_lstnr_cmd_stop_create() {
    ush_comm_lstnr_cmd_stop_t ret =
        (ush_comm_lstnr_cmd_stop_t)malloc(sizeof(struct lstnr_cmd_stop_s));

    if (!ret) {
        ush_log(LOG_LVL_ERROR, "cmd_stop msg mem alloc failed.");
        return NULL;
    }

    ret->desc.desc.port = USH_COMM_PORT_LSTNR;
    ret->desc.catalog   = USH_COMM_LSTNR_MSG_CATALOG_CMD_STOP;

    return ret;
}

ush_size_t
ush_comm_lstnr_cmd_stop_sizeof(void) {
    return sizeof(struct lstnr_cmd_stop_s);
}


ush_ret_t
ush_comm_lstnr_cmd_stop_destroy(ush_comm_lstnr_cmd_stop_t *pMsg) {
    if (!pMsg) {
        ush_log(LOG_LVL_INFO, "msg ptr null");
        return USH_RET_OK;
    }

    if (!(*pMsg)) {
        ush_log(LOG_LVL_INFO, "msg ptr null");
        return USH_RET_OK;
    }

    free(*pMsg);
    *pMsg = NULL;
    return USH_RET_OK;
}
