#ifndef USH_COMM_LISTENER_H
#define USH_COMM_LISTENER_H

#include "ush_def_pub.h"
#include "ush_type_pub.h"

#include "ush_comm_def.h"

typedef enum USH_COMM_LISTENER_MSG_CATALOG {
    USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU,
    USH_COMM_LISTENER_MSG_CATALOG_MAX
} USH_COMM_LISTENER_MSG_CATALOG;

typedef struct listener_msg_desc {
    USH_COMM_LISTENER_MSG_CATALOG  catalog;
} USH_COMM_MSG_PACKED ush_listener_msg_description;


#endif // USH_COMM_LISTENER_H
