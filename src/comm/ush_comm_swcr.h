#ifndef USH_COMM_SWCR_H
#define USH_COMM_SWCR_H

// header file for Service-Write-Client-Read pipe

#include "ush_type.h"

#define USH_COMM_SWCR_PATH_PREFIX "/FAW-HQ-COS-USH-SWCR-"
#define USH_COMM_SWCR_MSG_MAX_CNT  (10)
#define USH_COMM_SWCR_MSG_MAX_LEN (128)

// #define USH_COMM_SWCR_NAME_LEN (64)

// typedef struct ush_comm_swcr_msg_t {
//     ush_s8_t   data[USH_COMM_PIPE_SWCR_MSG_MAX_LEN];
// } ush_comm_swcr_msg_t;

typedef enum swcr_msg_catalog_t {
    USH_COMM_SWCR_MSG_CATALOG_HOWAREYOU = 0
} touch_msg_catalog_t;

typedef struct swcr_msg_desc_t {
    ush_s16_t  catalog;
} swcr_msg_desc_t;

typedef struct swcr_msg_howareyou_t {
    swcr_msg_desc_t desc;
    ush_s8_t foobar[16];
} swcr_msg_howareyou_t;


#endif // USH_COMM_SWCR_H
