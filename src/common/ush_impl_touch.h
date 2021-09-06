#ifndef USH_IMPL_COMMON_H_
#define USH_IMPL_COMMON_H_

#include "ush_type.h"

#define USH_IMPL_TOUCH_Q_PATH "/FAW-HQ-COS-USH-TOUCH_PIPE"
#define USH_IMPL_TOUCH_Q_MSG_MAX_CNT  (4)
#define USH_IMPL_TOUCH_Q_MSG_MAX_LEN (64)


#define USH_IMPL_TOUCH_Q_MSG_ID_LEN (4) // int32
#define USH_IMPL_TOUCH_Q_MSG_ID_HELLO (0)
#define USH_IMPL_TOUCH_Q_MSG_ID_HELLO_PROI (1)

#define USH_IMPL_TOUCH_Q_PIPENAME_LEN (32)

typedef struct ush_impl_touch_msg_t {
    ush_s32_t  id;
    ush_s8_t   name[USH_IMPL_TOUCH_Q_PIPENAME_LEN];
} ush_impl_touch_msg_t;


#endif // USH_IMPL_COMMON_H_
