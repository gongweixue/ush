#include "errno.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_def_pub.h"
#include "ush_log.h"
#include "ush_time.h"

#include "ushd_publish_fifo.h"
#include "ushd_publish_proc.h"
#include "ushd_publish_thread.h"


typedef struct publish_thread {
    pthread_t           tid;
    ushd_publish_fifo_t fifo;
    mqd_t               mq;
} *ushd_publish_thread_t;

static ush_ret_t publish_mq_open(ushd_publish_thread_t thread,
                                 const ush_char_t     *name);

void * ushd_publish_thread_entry(void *arg);

ushd_publish_thread_t
ushd_publish_thread_create(const ush_char_t *name) {
    ush_assert(name);
    ushd_publish_thread_t thread =
        (ushd_publish_thread_t)malloc(sizeof(struct publish_thread));
    if (!thread) {
        ushd_log(LOG_LVL_ERROR, "publish thread create failed");
        return NULL;
    }
    thread->tid  = USH_INVALID_TID; // invalid tid
    thread->mq   = USH_MQD_INVALID_VALUE;
    thread->fifo = NULL;

    if (USH_RET_OK != publish_mq_open(thread, name)) {
        ushd_log(LOG_LVL_ERROR, "open publish %s failed", name);
        ushd_log(LOG_LVL_INFO, "free thread %p", thread);
        free(thread);
        return NULL;
    }

    thread->fifo = ushd_publish_fifo_create();
    if (!thread->fifo) {
        ushd_log(LOG_LVL_ERROR, "publish %p create fifo failed", thread);
        ushd_log(LOG_LVL_INFO, "closed mq and free thread %p", thread);
        mq_close(thread->mq);
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
        ushd_log(LOG_LVL_INFO, "pop msg from publish fifo");

        PUBLISH_FIFO_MSG_TYPE ty = ((publish_fifo_msg_desc*)buf)->type;
        if (USHD_PUBLISH_FIFO_CMD_HOWAREYOU == ty) {
            publish_fifo_msg_howareyou *ptr = (publish_fifo_msg_howareyou *)buf;

            ushd_log(LOG_LVL_INFO, "sending msg %p to the mq", ptr);
            publish_proc_send_howareyou(thread->mq, ptr);
        }
    };
}


#define MQ_OPEN_RETRY_CNT          (3)
#define MQ_OPEN_RETRY_INTERVAL_MS  (10)

static ush_ret_t
publish_mq_open(ushd_publish_thread_t thread, const ush_char_t *name) {
    if (!name || USH_MQD_INVALID_VALUE != thread->mq) {
        return USH_RET_WRONG_PARAM;
    }

    ushd_log(LOG_LVL_DETAIL, "open publish %p mq %s", thread, name);
    for (int counter = 0; counter < MQ_OPEN_RETRY_CNT; ++counter) {
        thread->mq = mq_open(name, O_WRONLY);
        if (USH_MQD_INVALID_VALUE != thread->mq) { // done
            ush_log(LOG_LVL_INFO, "publish %p open done.", thread);
            return USH_RET_OK;

        } else { // failed
            if (ENOENT == errno) { // file has not been create
                ush_log(LOG_LVL_ERROR,
                        "retry after %d ms...",
                        MQ_OPEN_RETRY_INTERVAL_MS);
                ush_time_delay_ms(MQ_OPEN_RETRY_INTERVAL_MS);
                continue;
            } else {
                ush_log(LOG_LVL_ERROR, "publish %p open failed", thread);
                break;
            }
        }
    }
    return USH_RET_FAILED;
}
