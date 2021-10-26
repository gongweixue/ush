#include "errno.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "ush_assert.h"
#include "ush_comm_def.h"
#include "ush_log.h"
#include "ushd_realm_thread.h"

typedef struct ushd_realm_thread {
    pthread_t        tid;    // thread id for the ushd realm thread
    mqd_t            mq;     // mq handle
} * ushd_realm_thread_t;

static ush_ret_t realm_mq_open(ushd_realm_thread_t, const ush_char_t *);

static void *realm_thread_entry(void *arg);

ushd_realm_thread_t
ushd_realm_thread_create(const ush_char_t *name) {
    ushd_realm_thread_t thread =
        (ushd_realm_thread_t)malloc(sizeof(struct ushd_realm_thread));

    if (!thread) {
        ushd_log(LOG_LVL_FATAL, "create ushd_realm_thread_t failed");
        return NULL;
    }
    thread->tid = USH_INVALID_TID;
    thread->mq  = USH_INVALID_MQD_VALUE;

    if (USH_RET_OK != realm_mq_open(thread, name)) {
        ushd_log(LOG_LVL_ERROR, "open realm %s failed", name);
        ushd_log(LOG_LVL_INFO, "free thread %p", thread);
        free(thread);
        return NULL;
    }

    return thread;
}

ush_ret_t
ushd_realm_thread_start(ushd_realm_thread_t thread) {
    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, realm_thread_entry, thread)) {
        ushd_log(LOG_LVL_ERROR, "realm thread start failed");
        return USH_RET_FAILED;
    }
    if (0 != pthread_detach(tid)) {
        ushd_log(LOG_LVL_ERROR, "detach realm thread failed, %p.", thread);
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}


ush_ret_t
ushd_realm_thread_stop_destroy(ushd_realm_thread_t *pThread) {
    if (!pThread || !(*pThread)) {
        return USH_RET_OK;
    }

    if (USH_INVALID_MQD_VALUE != (*pThread)->mq) {
        mq_close((*pThread)->mq);
        (*pThread)->mq = USH_INVALID_MQD_VALUE;
    }

    if (USH_INVALID_TID != (*pThread)->tid) {
        pthread_cancel((*pThread)->tid);
        (*pThread)->tid = USH_INVALID_TID;
    }

    return USH_RET_OK;
}


static ush_ret_t
realm_mq_open(ushd_realm_thread_t thread, const ush_char_t *name) {
    if (!thread || !name || USH_INVALID_MQD_VALUE != thread->mq) {
        ushd_log(LOG_LVL_ERROR, "wrong param");
        return USH_RET_WRONG_PARAM;
    }

    struct mq_attr qAttr;
    memset(&qAttr, 0, sizeof(qAttr));
    qAttr.mq_maxmsg  = USH_COMM_REALM_Q_MSG_MAX_CNT;
    qAttr.mq_msgsize = USH_COMM_REALM_Q_MSG_MAX_LEN;
    thread->mq = mq_open(name, O_RDONLY | O_CREAT, S_IRWXU | S_IRWXG, &qAttr);

    if (USH_INVALID_MQD_VALUE == thread->mq) {
        ushd_log(LOG_LVL_ERROR, "ushd realm open returns invalid value");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}



#include "tch/sig/ush_comm_tch_sig_reg.h"

static void *
realm_thread_entry(void *arg) {
    ush_assert(arg);
    ushd_realm_thread_t thread = (ushd_realm_thread_t)arg;
    thread->tid = pthread_self();

    if (USH_INVALID_MQD_VALUE == thread->mq) {
        goto TERMINATE;
    }
    while(1) {
        static ush_char_t buf[USH_COMM_REALM_Q_MSG_MAX_LEN];
        ush_ssize_t rcv_sz = mq_receive(thread->mq, buf, sizeof(buf), NULL);
        if (-1 == rcv_sz) {
            switch (errno) {
            case EMSGSIZE:
                ushd_log(LOG_LVL_ERROR, "massage too long");
                break;

            case EBADF:
                ushd_log(LOG_LVL_ERROR, "bad mqd_t");
                break;

            case EINVAL:
                ushd_log(LOG_LVL_ERROR, "invalid ptr of buffer for receiving.");
                break;

            default:
                break;
            }
            continue; // skip to next loop.
        }

        ush_comm_tch_sig_reg_testpoint((ush_comm_tch_sig_reg_t)buf);
    };

TERMINATE:
    exit(0);
}
