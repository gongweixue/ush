#ifndef USH_COMM_TOUCH_H
#define USH_COMM_TOUCH_H

#include "ush_type_pub.h"

#include "ush_comm_def.h"

typedef enum USH_COMM_TOUCH_MSG_CATALOG {
    USH_COMM_TOUCH_MSG_CATALOG_HELLO = 0
} USH_COMM_TOUCH_MSG_CATALOG;

typedef struct touch_msg_desc {
    USH_COMM_TOUCH_MSG_CATALOG  catalog;
} ush_touch_msg_description USH_COMM_MSG_ALIGNMENT;



#endif // USH_COMM_TOUCH_H
