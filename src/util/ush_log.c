#include "pthread.h"

#include "ush_log.h"

pthread_mutex_t cs = PTHREAD_MUTEX_INITIALIZER;

void ush_log_cs_entry(void) {
    pthread_mutex_lock(&cs);
}

void ush_log_cs_exit(void) {
    pthread_mutex_unlock(&cs);
}
