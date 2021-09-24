#ifndef USH_COMM_LISTENER_H
#define USH_COMM_LISTENER_H

#include "ush_type_pub.h"

#include "ush_comm_def.h"

typedef enum USH_COMM_LISTENER_MSG_CATALOG {
    USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU = 0
} USH_COMM_LISTENER_MSG_CATALOG;

typedef struct listener_msg_desc {
    USH_COMM_LISTENER_MSG_CATALOG  catalog;
} ush_listener_msg_desc_t USH_COMM_MSG_ALIGNMENT;


#endif // USH_COMM_LISTENER_H
