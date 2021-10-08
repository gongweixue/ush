#ifndef USHD_DIST_THREAD_H
#define USHD_DIST_THREAD_H

#include "ush_type_pub.h"
#include "ushd_dist_fifo.h"

typedef struct dist_thread * ushd_dist_thread_t;

ushd_dist_thread_t ushd_dist_thread_create(const ush_char_t *name);

ush_ret_t          ushd_dist_thread_start(ushd_dist_thread_t thread);

ushd_dist_fifo_t   ushd_dist_thread_get_fifo(ushd_dist_thread_t thread);




#endif // USHD_DIST_THREAD_H
