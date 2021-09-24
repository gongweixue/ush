#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_log.h"

#include "ushd_publish_fifo.h"
#include "ushd_publish_proc.h"
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
    ush_assert(arg);
    ushd_publish_thread_t thread = (ushd_publish_thread_t)arg;
    thread->tid = pthread_self();

    // msg must be copy out to the external buffer, since the thread who
    // calls the proc may be blocked by the mqueue functions.
    // The concurrency of the fifo can not be ensure.
    // That is: producers would overwrite the buffer which is using by
    // consumers if the fifo is not locked.
    while(1) {
        ush_char_t buf[USHD_PUBLISH_FIFO_MSG_MAX_SIZE];
        if (0 == ushd_publish_fifo_pop(thread->fifo, buf, sizeof(buf))) {
            ushd_log(LOG_LVL_ERROR, "pop data len = 0");
            continue;
        }
        PUBLISH_FIFO_MSG_TYPE ty = ((publish_fifo_msg_desc*)buf)->type;
        if (USHD_PUBLISH_FIFO_CMD_HOWAREYOU == ty) {
            publish_fifo_msg_howareyou *ptr = (publish_fifo_msg_howareyou *)buf;
            publish_proc_send_howareyou(ptr);
        }
    };
}
