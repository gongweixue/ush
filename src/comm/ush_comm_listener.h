#ifndef USH_COMM_LISTENER_H
#define USH_COMM_LISTENER_H

// header file for Service-Write-Client-Read pipe

#include "ush_type_pub.h"

#define USH_COMM_LISTENER_PATH_PREFIX "/FAW-HQ-COS-USH-LISTENER-"
#define USH_COMM_LISTENER_MSG_MAX_CNT  (10)
#define USH_COMM_LISTENER_MSG_MAX_LEN (128)

#define USH_COMM_LISTENER_NAME_LEN_MAX (64)

// typedef struct ush_comm_listener_msg_t {
//     ush_s8_t   data[USH_COMM_PIPE_listener_MSG_MAX_LEN];
// } ush_comm_listener_msg_t;

typedef enum listener_msg_catalog_t {
    USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU = 0
} touch_msg_catalog_t;

typedef struct listener_msg_desc_t {
    ush_s16_t  catalog;
} listener_msg_desc_t;

typedef struct listener_msg_howareyou_t {
    listener_msg_desc_t desc;
    ush_s8_t foobar[16];
} listener_msg_howareyou_t;


#endif // USH_COMM_LISTENER_H
