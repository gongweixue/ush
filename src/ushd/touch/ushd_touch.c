
#include "fcntl.h"
#include "mqueue.h"
#include "pthread.h"
#include "string.h"
#include "unistd.h"

#include "ush_assert.h"
#include "ush_comm_touch.h"
#include "ush_log.h"
#include "ush_type_pub.h"

// #include "ushd_sw.h"
#include "ushd_touch.h"

// *** touch thread entry
static void *touch_entry(void *arg);


ush_ret_t ushd_touch_start() {
    // pthread_t tid;
    // if (0 != pthread_create(&tid, NULL, touch_entry, NULL)) {
    //     ush_log(USH_LOG_LVL_ERROR, "create touch daemon thread: failed.\n");
    //     return USH_RET_FAILED;
    // }

    // if (0 != pthread_detach(tid)) {
    //     ush_log(USH_LOG_LVL_ERROR, "detach touch daemon thread: failed.\n");
    //     return USH_RET_FAILED;
    // }

    return USH_RET_OK;
}


static void *touch_entry(void *arg) {

    // ushd_thread_set_tid(USHD_THREAD_TID_IDX_TOUCH, pthread_self());

    // struct mq_attr qAttr;
    // memset(&qAttr, 0, sizeof(qAttr));
    // qAttr.mq_maxmsg  = USH_IMPL_TOUCH_Q_MSG_MAX_CNT;
    // qAttr.mq_msgsize = USH_IMPL_TOUCH_Q_MSG_MAX_LEN;
    // mqd_t mq = mq_open(USH_IMPL_TOUCH_Q_PATH,
    //                    O_RDONLY | O_CREAT, // read end
    //                    S_IRWXU | S_IRWXG, // 0770
    //                    &qAttr);

    // if (-1 == mq) {
    //     ush_log(USH_LOG_LVL_ERROR, "open failed\n");
    // }

    // while (1) {
    //     char  buff[USH_IMPL_TOUCH_Q_MSG_MAX_LEN];
    //     ush_log(USH_LOG_LVL_INFO, "receiving touch\n");
    //     ush_ssize_t rcv_sz = mq_receive(mq, buff, sizeof(buff), NULL);

    //     if (-1 == rcv_sz) {
    //         ush_log(USH_LOG_LVL_ERROR, "ERROR rcv_sz\n");
    //         continue;
    //     }
    //     touch_dispatch(buff);
    // }

    return 0;
}
