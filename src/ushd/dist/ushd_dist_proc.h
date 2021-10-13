#ifndef USHD_DIST_PROC_H
#define USHD_DIST_PROC_H

#include "mqueue.h"

#include "ush_type_pub.h"
#include "ushd_dist_fifo_msg.h"

void dist_proc_send_hay(mqd_t mq, dist_fifo_msg_hay *msg);

#endif // USHD_DIST_PROC_H
