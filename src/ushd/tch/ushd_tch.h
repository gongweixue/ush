#ifndef USHD_TCH_H
#define USHD_TCH_H

#include "ush_type_pub.h"

typedef struct ushd_tch  * ushd_tch_t;

ush_ret_t ushd_tch_create(ushd_tch_t *pTouch);

ush_ret_t ushd_tch_open(ushd_tch_t touch);

ush_ret_t ushd_tch_close(ushd_tch_t touch);

ush_ret_t ushd_tch_receive(ushd_tch_t   touch,
                           ush_char_t  *dest,
                           ush_size_t   sz);

ush_ret_t ushd_tch_destroy_with_closing(ushd_tch_t *pTouch);


#endif // USHD_TCH_H
