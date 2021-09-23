#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_log.h"

#include "ushd_publish_fifo.h"
#include "ushd_publish_thread.h"


typedef struct publish_thread {
    pthread_t tid;
    ushd_publish_fifo_t fifo;
} *ushd_publish_thread_t;

void * ushd_publish_thread_entry(void *arg);

ushd_publish_thread_t
ushd_publish_thread_create() {
    ushd_publish_thread_t thread =
        (ushd_publish_thread_t)malloc(sizeof(struct publish_thread));
    if (!thread) {
        ushd_log(LOG_LVL_ERROR, "publish thread create failed");
        return NULL;
    }
    thread->tid = 0xFFFFFFFF; // invalid tid

    thread->fifo = ushd_publish_fifo_create();
    if (!thread->fifo) {
        ushd_log(LOG_LVL_ERROR, "publish %p create fifo failed", thread);
        free(thread);
        return NULL;
    }

    return thread;
}

ush_ret_t
ushd_publish_thread_start(ushd_publish_thread_t thread) {

    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, ushd_publish_thread_entry, thread)) {
        ushd_log(LOG_LVL_ERROR, "publish thread start failed");
        return USH_RET_FAILED;
    }
    if (0 != pthread_detach(tid)) {
        ushd_log(LOG_LVL_ERROR, "detach publish thread failed, %p.", thread);
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}

ushd_publish_fifo_t
ushd_publish_thread_get_fifo(ushd_publish_thread_t thread) {
    ush_assert(thread);
    if (!thread) {
        return NULL;
    }
    return thread->fifo;
}



void * ushd_publish_thread_entry(void *arg) {
    ushd_publish_thread_t thread = (ushd_publish_thread_t)arg;
    // set tid to the thread->tid;
    // get thread->fifo, and wait the cond, read the msg, and react.
    while(1) {};
}
