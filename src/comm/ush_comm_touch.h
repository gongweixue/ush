#ifndef USH_COMM_TOUCH_H
#define USH_COMM_TOUCH_H

#include "ush_type_pub.h"

#define USH_COMM_TOUCH_Q_PATH "/FAW-HQ-COS-USH-TOUCH_PIPE"
#define USH_COMM_TOUCH_Q_MSG_MAX_CNT  (10)
#define USH_COMM_TOUCH_Q_MSG_MAX_LEN (128)


typedef enum touch_msg_catalog_type {
    USH_COMM_TOUCH_MSG_CATALOG_HELLO = 0
} touch_msg_catalog_type;


typedef struct touch_msg_desc {
    touch_msg_catalog_type  catalog;
} ush_touch_msg_desc;



#endif // USH_COMM_TOUCH_H
