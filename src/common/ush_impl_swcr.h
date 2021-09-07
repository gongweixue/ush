#ifndef USH_IMPL_SWCR_H
#define USH_IMPL_SWCR_H

#include "ush_type.h"

#define USH_IMPL_PIPE_SWCR_PATH_PREFIX "/FAW-HQ-COS-USH-SWCR-"
#define USH_IMPL_PIPE_SWCR_MSG_MAX_CNT  (10)
#define USH_IMPL_PIPE_SWCR_MSG_MAX_LEN (128)

#define USH_IMPL_PIPE_SWCR_NAME_LEN (64)

typedef struct ush_impl_swcr_msg_t {
    ush_s8_t   data[USH_IMPL_PIPE_SWCR_MSG_MAX_LEN];
} ush_impl_swcr_msg_t;


#endif // USH_IMPL_SWCR_H
