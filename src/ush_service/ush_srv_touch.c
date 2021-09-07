#include "mqueue.h"
#include "string.h"
#include "fcntl.h"
#include "pthread.h"
#include "ush_impl_touch.h"
#include "ush_impl_log.h"
#include "ush_impl_protocol.h"

#include "ush_type.h"
#include "ush_srv_touch.h"
#include "ush_srv_thread.h"

static void *touch_daemon_entry(void *arg) {

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
        ush_log(USH_LOG_LVL_ERR, "open failed\n");
    }

    while (1) {
        ush_srv_thread_state_t state = USH_SRV_THREAD_STATE_NON;
        ush_srv_thread_state(USH_SRV_THREAD_TID_IDX_TOUCH, &state);
        if (USH_SRV_THREAD_STATE_STOP == state) {
            break;
        }

        char  buff[USH_IMPL_TOUCH_Q_MSG_MAX_LEN];
        ush_log(USH_LOG_LVL_INFO, "receiving touch\n");
        ush_ssize_t rcv_sz = mq_receive(mq, buff, sizeof(buff), NULL);

        if (-1 == rcv_sz) {
            ush_log(USH_LOG_LVL_ERR, "ERROR rcv_sz\n");
            continue;
        }

        {// process msg
            printf("%ld\n", rcv_sz);
            ush_impl_touch_msg_t *pTouch = (ush_impl_touch_msg_t *)buff;
            printf("%d %s\n", pTouch->id, pTouch->name);
        }


    }

    if (mq_close(mq) == -1) {
        ush_log(USH_LOG_LVL_ERR, "close touch queue failed\n");
    }
    if (mq_unlink(USH_IMPL_TOUCH_Q_PATH) == -1) {
        ush_log(USH_LOG_LVL_ERR, "unlink touch queue failed\n");
    }

    return 0;
}

ush_ret_t ush_srv_thread_start_touch_daemon() {
    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, touch_daemon_entry, NULL)) {
        ush_log(USH_LOG_LVL_ERR, "create touch daemon thread: failed.\n");
        return USH_RET_FAILED;
    }

    if (0 != pthread_detach(tid)) {
        ush_log(USH_LOG_LVL_ERR, "detach touch daemon thread: failed.\n");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}
