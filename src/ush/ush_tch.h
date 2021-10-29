#ifndef USH_TCH_H
#define USH_TCH_H

typedef struct ush_tch_s *  ush_tch_t;

#include "time.h"

#include "ush_type_pub.h"
#include "tch/ush_comm_tch_hello.h"

ush_ret_t ush_tch_send(const ush_tch_t touch,
                       const ush_char_t *ptr,
                       ush_size_t sz,
                       ush_u32_t prio);

ush_ret_t ush_tch_send_hello(const ush_tch_t              touch,
                             const ush_comm_tch_hello_t   hello,
                             const struct timespec       *pDL);

ush_ret_t ush_tch_alloc(ush_tch_t *pTouch);

ush_ret_t ush_tch_open(ush_tch_t touch);

ush_ret_t ush_tch_destroy_with_closing(ush_tch_t *pTouch);


#endif // USH_TCH_H
