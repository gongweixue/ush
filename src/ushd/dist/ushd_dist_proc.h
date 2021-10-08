#ifndef USHD_DIST_PROC_H
#define USHD_DIST_PROC_H

#include "mqueue.h"

#include "ushd_dist_fifo_msg.h"

void dist_proc_send_howareyou(mqd_t mq, dist_fifo_msg_howareyou *msg);

#endif // USHD_DIST_PROC_H
