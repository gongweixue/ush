#ifndef USH_PIPE_H
#define USH_PIPE_H

#include "time.h"

typedef struct timespec timespec;

typedef struct ush_pipe_ack_sync_handle_t {
    pthread_cond_t cond;
    pthread_condattr_t condattr;
    pthread_mutex_t mutex;

} ush_pipe_ack_sync_handle_t;

#endif // USH_PIPE_H
