#ifndef USH_PIPE_TOUCH_H
#define USH_PIPE_TOUCH_H

#include "time.h"
#include "ush_hello.h"

typedef struct touch_t*    ush_touch_t;

ush_ret_t ush_touch_send_hello(const ush_touch_t     touch,
                               const ush_hello_msg_t *pHello,
                               const struct timespec *pDL);

ush_ret_t ush_touch_open(ush_touch_t touch);

ush_ret_t ush_touch_close(ush_touch_t touch);

ush_ret_t ush_touch_alloc(ush_touch_t *pTouch);

ush_ret_t ush_touch_destroy_with_closing(ush_touch_t touch);


#endif // USH_PIPE_TOUCH_H
