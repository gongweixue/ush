#include "assert.h"
#include "mqueue.h"
#include "ush_pipe_cr.h"
#include "pthread.h"
#include "string.h"
#include "ush_log.h"
#include "ush_comm_swcr.h"

static void *cr_entry(void *arg) {
    // // TODO:register the thread state??

    // // param valid
    // const char *pCrName = (const char *)arg;
    // assert(pCrName);
    // int lenFullName = strlen(USH_COMM_PIPE_SWCR_PATH_PREFIX) + strlen(pCrName);
    // if (USH_COMM_PIPE_SWCR_NAME_LEN <= lenFullName) {
    //     ush_log(USH_LOG_LVL_ERR, "CR name too long!!!\n");
    //     pthread_exit(NULL);
    // }

    // //create the CR queue to the service
    // struct mq_attr qAttr;
    // memset(&qAttr, 0, sizeof(qAttr));
    // qAttr.mq_maxmsg  = USH_COMM_PIPE_SWCR_MSG_MAX_CNT;
    // qAttr.mq_msgsize = USH_COMM_PIPE_SWCR_MSG_MAX_LEN;
    // char name[USH_COMM_PIPE_SWCR_NAME_LEN] = USH_COMM_PIPE_SWCR_PATH_PREFIX;
    // strcat(name, pCrName);
    // mqd_t mq = mq_open(name,
    //                    O_RDONLY | O_CREAT, // read end
    //                    S_IRWXU | S_IRWXG, // 0770
    //                    &qAttr);

    // if (-1 == mq) {
    //     ush_log(USH_LOG_LVL_ERR, "CR mqueue open failed\n");
    // }

    // while (1) {

    //     char  buff[USH_COMM_PIPE_SWCR_MSG_MAX_LEN];
    //     ush_log(USH_LOG_LVL_INFO, "CR queue receiving \n");
    //     ush_ssize_t rcv_sz = mq_receive(mq, buff, sizeof(buff), NULL);

    //     if (-1 == rcv_sz) {
    //         ush_log(USH_LOG_LVL_ERR, "CR mqueue error: rcv_sz=-1\n");
    //         continue;
    //     }

    //     // cr_dispatch();

    //     // {// process msg
    //     //     printf("%ld received\n", rcv_sz);
    //     //     ush_impl_swcr_msg_t *pCrMsg = (ush_impl_swcr_msg_t *)buff;
    //     //     printf("%s\n", pCrMsg->data);
    //     // }
    // }

    return 0;
}

ush_ret_t ush_pipe_cr_open(const char *pName) {
    if (!pName) {
        return USH_RET_WRONG_PARAM;
    }

    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, cr_entry, (void*)pName)) {
        ush_log(USH_LOG_LVL_ERR, "create cr thread: failed.\n");
        return USH_RET_FAILED;
    }

    // pthread_cond_wait()

    if (0 != pthread_detach(tid)) {
        ush_log(USH_LOG_LVL_ERR, "detach cr thread: failed.\n");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}

ush_ret_t ush_pipe_cr_close() {
    return USH_RET_OK;
}
