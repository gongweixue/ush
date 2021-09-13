
#include "fcntl.h"
#include "mqueue.h"
#include "pthread.h"
#include "string.h"
#include "unistd.h"

#include "ush_comm_protocol.h"
#include "ush_comm_touch.h"
#include "ush_log.h"
#include "ush_srv_sw.h"
#include "ush_srv_touch.h"
#include "ush_type_pub.h"

// touch thread entry
static void *touch_entry(void *arg);

// dispatch into the internal queues for dealling with.
static void touch_dispatch(const void *pBuf);

ush_ret_t ush_srv_touch_start() {
    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, touch_entry, NULL)) {
        ush_log(USH_LOG_LVL_ERROR, "create touch daemon thread: failed.\n");
        return USH_RET_FAILED;
    }

    if (0 != pthread_detach(tid)) {
        ush_log(USH_LOG_LVL_ERROR, "detach touch daemon thread: failed.\n");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}


static void *touch_entry(void *arg) {

    ush_srv_thread_set_tid(USH_SRV_THREAD_TID_IDX_TOUCH, pthread_self());

    struct mq_attr qAttr;
    memset(&qAttr, 0, sizeof(qAttr));
    qAttr.mq_maxmsg  = USH_IMPL_TOUCH_Q_MSG_MAX_CNT;
    qAttr.mq_msgsize = USH_IMPL_TOUCH_Q_MSG_MAX_LEN;
    mqd_t mq = mq_open(USH_IMPL_TOUCH_Q_PATH,
                       O_RDONLY | O_CREAT, // read end
                       S_IRWXU | S_IRWXG, // 0770
                       &qAttr);

    if (-1 == mq) {
        ush_log(USH_LOG_LVL_ERROR, "open failed\n");
    }

    while (1) {
        char  buff[USH_IMPL_TOUCH_Q_MSG_MAX_LEN];
        ush_log(USH_LOG_LVL_INFO, "receiving touch\n");
        ush_ssize_t rcv_sz = mq_receive(mq, buff, sizeof(buff), NULL);

        if (-1 == rcv_sz) {
            ush_log(USH_LOG_LVL_ERROR, "ERROR rcv_sz\n");
            continue;
        }
        touch_dispatch(buff);
    }

    return 0;
}

static void touch_dispatch(const void *pBuf) {
    const ush_comm_touch_msg_t *pMsg = (const ush_comm_touch_msg_t *)pBuf;
    // // process msg
        // printf("receive %ld bytes\n", rcv_sz);
        // ush_impl_touch_msg_t *pTouch = (ush_impl_touch_msg_t *)buff;
        // printf("%d %s\n", pTouch->id, pTouch->name);

        // if (USH_COMM_PROTOCOL_TOUCH_ID_PING == pTouch->id) {
        //     ush_srv_sw_open(pTouch->name);
        // }
}
