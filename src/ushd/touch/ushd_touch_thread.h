#ifndef USHD_TOUCH_THREAD_H
#define USHD_TOUCH_THREAD_H

#include "pthread.h"

#include "ush_type_pub.h"

typedef struct ushd_touch_thread * ushd_touch_thread_t;


ushd_touch_thread_t ushd_touch_thread_singleton();

ush_ret_t ushd_touch_thread_set_id(pthread_t tid);



void *ushd_touch_thread_entry(void *arg);

ush_ret_t ushd_touch_thread_start();

#endif //USHD_TOUCH_THREAD_H
