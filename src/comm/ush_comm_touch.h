#ifndef USH_COMM_TOUCH_H
#define USH_COMM_TOUCH_H

#include "ush_type_pub.h"

#include "ush_comm_def.h"

typedef enum USH_COMM_TOUCH_MSG_CATALOG {
    USH_COMM_TOUCH_MSG_CATALOG_HELLO,
    USH_COMM_TOUCH_MSG_CATALOG_SIG,
    USH_COMM_TOUCH_MSG_CATALOG_MAX
} USH_COMM_TOUCH_MSG_CATALOG;

typedef struct touch_msg_desc {
    ush_comm_port_description   desc;
    USH_COMM_TOUCH_MSG_CATALOG  catalog;
} USH_COMM_MSG_PACKED ush_touch_msg_description;



#endif // USH_COMM_TOUCH_H
