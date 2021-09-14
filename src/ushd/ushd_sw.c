
#include "mqueue.h"
#include "pthread.h"
#include "string.h"
#include "unistd.h"

#include "ush_comm_listener.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ushd_sw.h"

static void *sw_entry(void *arg);

ush_ret_t ushd_sw_open(const char *pName) {
    if (!pName) {
        return USH_RET_WRONG_PARAM;
    }

    pthread_t tid;
    if (0 != pthread_create(&tid, NULL, sw_entry, (void*)pName)) {
        ushd_log(LOG_LVL_ERROR, "create sw thread: failed.\n");
        return USH_RET_FAILED;
    }

    if (0 != pthread_detach(tid)) {
        ushd_log(LOG_LVL_ERROR, "detach sw thread: failed.\n");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}
static void *sw_entry(void *arg) {

    char buf[64] = USH_COMM_PIPE_SWCR_PATH_PREFIX;
    strcat(buf, (char *)arg);
    ushd_log(LOG_LVL_INFO, "pong back\n");
    mqd_t mq = mq_open(buf, O_WRONLY);
    ush_s32_t res = mq_send(mq, "pong", 3, USH_COMM_PROTOCOL_TOUCH_ID_PONG_PROI);

    while (1) {
        printf("sw thread running, waiting sending request...\n");
        sleep(2);
    }

    return 0;
}
