#ifndef USH_IMPL_TOUCH_H
#define USH_IMPL_TOUCH_H

#include "ush_type.h"

#define USH_IMPL_TOUCH_Q_PATH "/FAW-HQ-COS-USH-TOUCH_PIPE"
#define USH_IMPL_TOUCH_Q_MSG_MAX_CNT  (10)
#define USH_IMPL_TOUCH_Q_MSG_MAX_LEN (128)

#define USH_IMPL_TOUCH_Q_NAME_LEN (32)

typedef struct ush_impl_touch_msg_t {
    ush_s32_t  id;
    ush_s8_t   name[USH_IMPL_TOUCH_Q_NAME_LEN];
} ush_impl_touch_msg_t;


#endif // USH_IMPL_TOUCH_H
