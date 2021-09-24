#ifndef USHD_PUBLISH_THREAD_H
#define USHD_PUBLISH_THREAD_H

#include "ush_type_pub.h"
#include "ushd_publish_fifo.h"

typedef struct publish_thread * ushd_publish_thread_t;

ushd_publish_thread_t ushd_publish_thread_create(const ush_char_t *name);

ush_ret_t            ushd_publish_thread_start(ushd_publish_thread_t thread);

ushd_publish_fifo_t  ushd_publish_thread_get_fifo(ushd_publish_thread_t thread);




#endif // USHD_PUBLISH_THREAD_H
