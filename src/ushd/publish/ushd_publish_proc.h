#ifndef USHD_PUBLISH_PROC_H
#define USHD_PUBLISH_PROC_H

#include "mqueue.h"

#include "ushd_publish_fifo_msg.h"

void publish_proc_send_howareyou(mqd_t mq, publish_fifo_msg_howareyou *msg);

#endif // USHD_PUBLISH_PROC_H
