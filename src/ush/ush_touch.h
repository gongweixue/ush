#ifndef USH_TOUCH_H
#define USH_TOUCH_H

typedef struct ush_touch *  ush_touch_t;

#include "time.h"

#include "ush_comm.h"

ush_ret_t ush_touch_send(const ush_touch_t touch,
                         const ush_char_t *ptr,
                         ush_size_t        sz);

ush_ret_t ush_touch_send_hello(const ush_touch_t          touch,
                               const ush_comm_hello_msg_t hello,
                               const struct timespec     *pDL);

ush_ret_t ush_touch_alloc(ush_touch_t *pTouch);

ush_ret_t ush_touch_open(ush_touch_t touch);

ush_ret_t ush_touch_close(ush_touch_t touch);

ush_ret_t ush_touch_destroy_with_closing(ush_touch_t *pTouch);


#endif // USH_TOUCH_H
