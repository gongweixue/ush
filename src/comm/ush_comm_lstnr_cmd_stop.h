#ifndef USH_COMM_LSTNR_CMD_STOP_H
#define USH_COMM_LSTNR_CMD_STOP_H

#include "ush_cb_pub.h"

typedef struct lstnr_cmd_stop_s * ush_comm_lstnr_cmd_stop_t;

ush_comm_lstnr_cmd_stop_t ush_comm_lstnr_cmd_stop_create(void);

ush_size_t ush_comm_lstnr_cmd_stop_sizeof(void);

ush_ret_t ush_comm_lstnr_cmd_stop_destroy(ush_comm_lstnr_cmd_stop_t *pMsg);

#endif // USH_COMM_LSTNR_CMD_STOP_H
