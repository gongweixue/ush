#ifndef USH_COMM_LISTENER_H
#define USH_COMM_LISTENER_H

// header file for Service-Write-Client-Read pipe

#include "ush_type_pub.h"

#define USH_COMM_LISTENER_Q_PATH_PREFIX "/FAW-HQ-COS-USH-LISTENER-"
#define USH_COMM_LISTENER_Q_MSG_MAX_CNT  (10)
#define USH_COMM_LISTENER_Q_MSG_MAX_LEN (128)

#define USH_COMM_LISTENER_Q_NAME_LEN_MAX (64)

#define USH_COMM_LISTENER_Q_MSG_ALIGNMENT __attribute__((aligned(1)))

typedef enum USH_COMM_LISTENER_MSG_CATALOG {
    USH_COMM_LISTENER_MSG_CATALOG_HOWAREYOU = 0
} USH_COMM_LISTENER_MSG_CATALOG;

typedef struct listener_msg_desc {
    USH_COMM_LISTENER_MSG_CATALOG  catalog;
} listener_msg_desc_t USH_COMM_LISTENER_Q_MSG_ALIGNMENT;

// typedef struct listener_msg_howareyou {
//     listener_msg_desc_t desc;
//     ush_s8_t foobar[16];
// } listener_msg_howareyou;


#endif // USH_COMM_LISTENER_H
