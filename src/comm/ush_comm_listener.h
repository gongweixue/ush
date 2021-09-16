#ifndef USH_COMM_LISTENER_H
#define USH_COMM_LISTENER_H

// header file for Service-Write-Client-Read pipe

#include "ush_type_pub.h"

#define USH_COMM_LISTENER_PATH_PREFIX "/FAW-HQ-COS-USH-LISTENER-"
#define USH_COMM_LISTENER_MSG_MAX_CNT  (10)
#define USH_COMM_LISTENER_MSG_MAX_LEN (128)

#define USH_COMM_LISTENER_NAME_LEN_MAX (64)


typedef enum listener_msg_catalog {
    USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU = 0
} listener_msg_catalog;

typedef struct listener_msg_desc {
    listener_msg_catalog  catalog;
} listener_msg_desc;

// typedef struct listener_msg_howareyou {
//     listener_msg_desc desc;
//     ush_s8_t foobar[16];
// } listener_msg_howareyou;


#endif // USH_COMM_LISTENER_H
