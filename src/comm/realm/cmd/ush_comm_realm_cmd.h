#ifndef USH_COMM_REALM_CMD_H
#define USH_COMM_REALM_CMD_H

#include "ush_type_pub.h"

typedef enum {
    USH_COMM_REALM_CMD_ID_CLOSE,
    USH_COMM_REALM_CMD_ID_INVALID
} USH_COMM_REALM_CMD_ID;

typedef struct comm_realm_cmd_s * ush_comm_realm_cmd_t;


ush_ret_t
ush_comm_realm_cmd_create(ush_comm_realm_cmd_t *pMsg, USH_COMM_REALM_CMD_ID id);

USH_COMM_REALM_CMD_ID
ush_comm_realm_cmd_id_of(const ush_comm_realm_cmd_t msg);

ush_size_t
ush_comm_realm_cmd_sizeof(void);

ush_ret_t
ush_comm_realm_cmd_destroy(ush_comm_realm_cmd_t *msg);

void
ush_comm_realm_cmd_testpoint(const ush_comm_realm_cmd_t msg);

#endif // USH_COMM_REALM_CMD_H
