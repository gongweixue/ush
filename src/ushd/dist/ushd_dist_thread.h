#ifndef USHD_DIST_THREAD_H
#define USHD_DIST_THREAD_H

#include "ush_type_pub.h"
#include "ushd_dist_fifo.h"

typedef struct dist_thread * ushd_dist_thread_t;

ushd_dist_thread_t ushd_dist_thread_create(const ush_char_t *name);

ush_ret_t          ushd_dist_thread_start(ushd_dist_thread_t thread);

ushd_dist_fifo_t   ushd_dist_thread_get_fifo(ushd_dist_thread_t thread);

ush_ret_t
ushd_dist_thread_send_msg(ushd_dist_thread_t thread,
                          const ush_pvoid_t  buf,
                          ush_size_t         sz,
                          ush_u32_t          prio);

#endif // USHD_DIST_THREAD_H
