#ifndef USH_COMM_LSTNR_H
#define USH_COMM_LSTNR_H

#include "ush_def_pub.h"
#include "ush_type_pub.h"

#include "ush_comm_def.h"

typedef enum USH_COMM_LSTNR_MSG_CATALOG {
    USH_COMM_LSTNR_MSG_CATALOG_HOWAREYOU,
    USH_COMM_LSTNR_MSG_CATALOG_MAX
} USH_COMM_LSTNR_MSG_CATALOG;

typedef struct lstnr_msg_desc {
    ush_comm_port_d                desc;
    USH_COMM_LSTNR_MSG_CATALOG  catalog;
} USH_COMM_MSG_PACKED ush_comm_lstnr_msg_d;


#endif // USH_COMM_LSTNR_H
