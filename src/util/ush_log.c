#include "pthread.h"

pthread_mutex_t cs = PTHREAD_MUTEX_INITIALIZER;

void ush_log_cs_entry() {
    pthread_mutex_lock(&cs);
}

void ush_log_cs_exit() {
    pthread_mutex_unlock(&cs);
}
