
#include "errno.h"
#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_log.h"

#include "ush_comm_desc.h"

#include "proc/ush_lstnr_proc.h"
#include "ush_lstnr.h"
#include "ush_sync.h"

typedef struct ush_lstnr_s {
    mqd_t      mq;
    pthread_t  tid;
} * ush_lstnr_t;

static void *    lstnr_thread_entry(void *arg);
static ush_ret_t ushd_lstnr_receive(mqd_t mq, ush_char_t *dest, size_t sz);

ush_ret_t
ush_lstnr_open_start(ush_lstnr_t *pPtr, const ush_char_t *fullname) {
    ush_assert(pPtr && fullname);
    *pPtr = NULL;

    ush_lstnr_t tmp = (ush_lstnr_t)malloc(sizeof(struct ush_lstnr_s));
    if (!tmp) {
        ush_log(LOG_LVL_FATAL, "listener alloc failed");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_INFO, "allocate memory for listener %p", tmp);

    tmp->tid = USH_INVALID_TID;

    // open the mqueue
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg  = USH_COMM_LSTNR_Q_MSG_MAX_CNT;
    attr.mq_msgsize = USH_COMM_LSTNR_Q_MSG_MAX_LEN;
    tmp->mq = mq_open(fullname, O_RDONLY | O_CREAT, S_IRWXU | S_IRWXG, &attr);
    if (USH_INVALID_MQD_VALUE == tmp->mq) {
        ush_log(LOG_LVL_FATAL, "%s can not open, errno:%d", fullname, errno);
        free(tmp);
        return USH_RET_FAILED;
    }
    ush_log(LOG_LVL_INFO, "the lstnr queue opened, %p", tmp);


    if (0 != pthread_create(&tmp->tid, NULL, lstnr_thread_entry, tmp)) {
        ush_log(LOG_LVL_FATAL, "lstnr %s thread start failed", fullname);
        mq_close(tmp->mq);
        free(tmp);
        return USH_RET_FAILED;
    }

    if (0 != pthread_detach(tmp->tid)) {
        ush_log(LOG_LVL_FATAL, "detach lstnr %s thread failed", fullname);
        pthread_cancel(tmp->tid);
        mq_close(tmp->mq);
        free(tmp);
        return USH_RET_FAILED;
    }

    *pPtr = tmp;

    return USH_RET_OK;
}

ush_ret_t
ush_lstnr_stop_close(ush_lstnr_t *pPtr) {
    ush_assert(pPtr);
    if (!(*pPtr)) {
        ush_log(LOG_LVL_INFO, "NULL to be destroy");
        return USH_RET_OK;
    }

    if (USH_INVALID_TID != (*pPtr)->tid) {
        pthread_cancel((*pPtr)->tid);
        (*pPtr)->tid = USH_INVALID_TID;
        ush_log(LOG_LVL_INFO, "listener thread %lu cancel", (*pPtr)->tid);
    }

    if (USH_INVALID_MQD_VALUE != (*pPtr)->mq) {
        mq_close((*pPtr)->mq);
        (*pPtr)->mq = USH_INVALID_MQD_VALUE;
        ush_log(LOG_LVL_INFO, "listener mq closed.");
    }

    free(*pPtr);
    *pPtr = NULL;
    ush_log(LOG_LVL_INFO, "listener %p destoried", *pPtr);

    return USH_RET_OK;
}

static void *
lstnr_thread_entry(void *arg) {
    ush_assert(arg);
    ush_lstnr_t listener = (ush_lstnr_t)arg;
    ush_assert(pthread_self() == listener->tid);

    while (1) {
        ush_char_t buf[USH_COMM_LSTNR_Q_MSG_MAX_LEN];
        ush_ret_t res = ushd_lstnr_receive(listener->mq, buf, sizeof(buf));
        if (USH_RET_OK != res) {
            ush_log(LOG_LVL_ERROR, "listener %p receive msg failed", listener);
            continue;
        }
        ush_log(LOG_LVL_INFO, "listener %p receive msg %p", listener, buf);

        if (USH_COMM_PORT_LSTNR != ((ush_comm_d*)buf)->port) {
            continue;
        }

        ush_lstnr_proc((ush_comm_lstnr_msg_d *)buf);
    }

    return 0;
}

static ush_ret_t
ushd_lstnr_receive(mqd_t mq, ush_char_t *dest, size_t sz) {
    ush_assert(sz >= USH_COMM_LSTNR_Q_MSG_MAX_LEN);
    if (USH_INVALID_MQD_VALUE == mq || !dest) {
        ushd_log(LOG_LVL_ERROR, "ushd touch not open");
        return USH_RET_WRONG_PARAM;
    }

    ushd_log(LOG_LVL_INFO, "receiving from listener thread......");
    ush_ssize_t rcv_sz = mq_receive(mq, dest, sz, NULL);

    if (-1 == rcv_sz) {
        switch (errno) {
        case EMSGSIZE:
            ush_log(LOG_LVL_ERROR, "massage too long from listener");
            break;

        case EBADF:
            ush_log(LOG_LVL_ERROR, "bad mqd_t for listener");
            break;

        case EINVAL:
            ush_log(LOG_LVL_ERROR, "invalid ptr of buffer for receiving.");
            break;

        default:
            break;
        }

        ush_log(LOG_LVL_ERROR, "received ret value is -1");
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_INFO, "ushd listener got the data");

    return USH_RET_OK;
}
