#include "errno.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_log.h"

#include "ush_comm_def.h"
#include "ush_comm_desc.h"
#include "realm/ush_comm_realm.h"
#include "realm/cmd/ush_comm_realm_cmd.h"
#include "realm/sig/ush_comm_realm_sigreg.h"

#include "sched/ushd_sched_fifo.h"

#include "ushd_realm_thread.h"

typedef struct ushd_realm_thread_s {
    pthread_t        tid;    // thread id for the ushd realm thread
    mqd_t            mq;     // mq handle
    ush_char_t       fullname[USH_COMM_CONN_FULL_NAME_LEN_MAX];
} * ushd_realm_thread_t;

static ush_ret_t realm_mq_open(ushd_realm_thread_t, const ush_char_t *);

static void *realm_thread_entry(void *arg);

ushd_realm_thread_t
ushd_realm_thread_create(const ush_char_t *name) {
    ushd_realm_thread_t thread =
        (ushd_realm_thread_t)malloc(sizeof(struct ushd_realm_thread_s));

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
    strncpy(thread->fullname, name, sizeof(thread->fullname));

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
ushd_realm_thread_request_stop(ushd_realm_thread_t *pThread) {
    if (!pThread || !(*pThread)) {
        return USH_RET_OK;
    }

    // realm appears not healthy
    mqd_t      mq = (*pThread)->mq;
    pthread_t tid = (*pThread)->tid;
    if (USH_INVALID_MQD_VALUE == mq || USH_INVALID_TID == tid) {
        ushd_log(LOG_LVL_FATAL, "wrong param");
        if (USH_INVALID_TID != tid) {
            // realm thread can't receive the msg
            pthread_cancel(tid);
            (*pThread)->tid = USH_INVALID_TID;

        }
        if (USH_INVALID_MQD_VALUE != mq) {
            // no running realm thread to closes this mq
            mq_close(mq);
            mq_unlink((*pThread)->fullname);
            (*pThread)->mq = USH_INVALID_MQD_VALUE;
        }

        free(*pThread);
        *pThread = NULL;
        return USH_RET_WRONG_PARAM;
    }

    // prepare msg
    ush_size_t sz = ush_comm_realm_cmd_sizeof();
    if (sz >= USH_COMM_REALM_Q_MSG_MAX_LEN) {
        ush_log(LOG_LVL_FATAL, "msg tooooooooo long!!!!");
        return USH_RET_FAILED;
    }

    ush_u32_t prio = USH_COMM_REALM_SEND_PRIO_CMD;

    ush_comm_realm_cmd_t cmd = NULL;
    ush_ret_t ret = ush_comm_realm_cmd_create(&cmd,USH_COMM_REALM_CMD_ID_CLOSE);
    if (USH_RET_OK != ret) {
        ush_log(LOG_LVL_ERROR, "out of mem for realm_cmd create");
        return USH_RET_OUT_OF_MEM;
    }

    // send a request into the queue with timeout to avoid thread blocked
    struct timespec ts;
    clock_gettime(USH_CLOCK_ID, &ts);
    ts.tv_sec += USH_REALM_QUEUE_SENDING_TIMEOUT_SEC;
    int i = USH_MQ_TIMEDSEND((*pThread)->mq, (const ush_char_t*)cmd, sz, prio, &ts);
    if (-1 == i) {
        ush_log(LOG_LVL_FATAL, "send realm msg failed.");
        return USH_RET_FAILED;
    }

    // DO NOT free thread object, cause the real deatroy action will
    // be performed in realm thread itself
    *pThread = NULL;

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

        if (USH_COMM_PORT_REALM != ((ush_comm_d *)buf)->port) {
            ushd_log(LOG_LVL_ERROR, "msg type not support");
            continue;
        }

        ushd_log(LOG_LVL_INFO, "msg arrived");

        // need to exit or not
        ush_comm_realm_msg_d *pRealm = (ush_comm_realm_msg_d*)buf;
        if (USH_COMM_REALM_MSG_CATALOG_CMD == pRealm->catalog) {
            ush_comm_realm_cmd_t cmd = (ush_comm_realm_cmd_t)buf;
            if (USH_COMM_REALM_CMD_ID_CLOSE == ush_comm_realm_cmd_id_of(cmd)) {
                if (USH_INVALID_MQD_VALUE != thread->mq) {
                    mq_close(thread->mq);
                    thread->mq = USH_INVALID_MQD_VALUE;
                    mq_unlink(thread->fullname);
                    thread->fullname[0] = '\0';
                    free(thread);
                    goto TERMINATE;
                }
            }
        }

        // push msg
        ushd_sched_fifo_t fifo = ushd_sched_fifo_singleton();
        if (USH_RET_OK != ushd_sched_fifo_push(fifo, buf, sizeof(buf))) {
            ushd_log(LOG_LVL_ERROR, "sched fifo push failed %p", buf);
            continue;
        }
    };

TERMINATE:
    pthread_exit(NULL);
}
