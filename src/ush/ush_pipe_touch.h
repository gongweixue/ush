#ifndef USH_PIPE_TOUCH_H
#define USH_PIPE_TOUCH_H

#include "time.h"
#include "ush_pipe_hello.h"
#include "mqueue.h"

typedef struct ush_pipe_touch_t {
    mqd_t mq;
} ush_pipe_touch_t;

ush_ret_t ush_pipe_touch_send_hello(const ush_pipe_touch_t *pTouch,
                                    const ush_pipe_msg_hello_t *pHello,
                                    const struct timespec *pDL);


#endif // USH_PIPE_TOUCH_H
