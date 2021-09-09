#include "mqueue.h"
#include "assert.h"
#include "errno.h"
#include "ush_log.h"
#include "ush_pipe_touch.h"


ush_ret_t
ush_pipe_touch_send_hello(const ush_pipe_touch_t *pTouch,
                          const ush_pipe_msg_hello_t *pHello,
                          const  struct timespec *pDL) {
    assert(pTouch && pHello);
    ush_ret_t ret = USH_RET_OK;
    const ush_char_t *pMsg = (const ush_char_t *)pHello;

    if (pDL) { // with timeout
        if (-1 == mq_timedsend(pTouch->mq, pMsg, sizeof(ush_pipe_msg_hello_t), 0, pDL)) {
            if ((errno == EINTR) || (errno == ETIMEDOUT)) {
                ush_log(USH_LOG_LVL_ERR, "send hello timeout\n");
                ret = USH_RET_TIMEOUT;
            } else {
                ush_log(USH_LOG_LVL_ERR, "send hello failed.\n");
                ret = USH_RET_FAILED;
            }
        }
    } else {
        if (-1 == mq_send(pTouch->mq, pMsg, sizeof(ush_pipe_msg_hello_t), 0)) {
            ush_log(USH_LOG_LVL_ERR, "send hello failed.\n");
            ret = USH_RET_FAILED;
        }
    }

    return USH_RET_OK;
}
