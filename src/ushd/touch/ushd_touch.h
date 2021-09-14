#ifndef USHD_TOUCH_H
#define USHD_TOUCH_H

#include "ush_type_pub.h"

typedef struct ushd_touch  * ushd_touch_t;

ush_ret_t ushd_touch_alloc(ushd_touch_t *pTouch);

ush_ret_t ushd_touch_open(ushd_touch_t touch);

ush_ret_t ushd_touch_close(ushd_touch_t touch);

ush_ret_t ushd_touch_destroy_with_closing(ushd_touch_t *pTouch);


#endif // USHD_TOUCH_H
