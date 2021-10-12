#ifndef USHD_TCH_THREAD_H
#define USHD_TCH_THREAD_H

#include "pthread.h"

#include "ush_type_pub.h"

typedef struct ushd_tch_thread * ushd_tch_thread_t;


ushd_tch_thread_t ushd_tch_thread_singleton();

ush_ret_t ushd_tch_thread_set_id(pthread_t tid);

ush_ret_t ushd_tch_thread_start();

#endif //USHD_TOCH_THREAD_H
