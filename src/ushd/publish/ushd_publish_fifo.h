#ifndef USHD_PUBLISH_FIFO_H
#define USHD_PUBLISH_FIFO_H

#include "ush_type_pub.h"

#include "ushd_publish_fifo_msg.h"

typedef struct publish_fifo * ushd_publish_fifo_t;

ushd_publish_fifo_t ushd_publish_fifo_create();

ush_ret_t ushd_publish_fifo_push(ushd_publish_fifo_t fifo,
                                 publish_fifo_msg_desc *pmsg,
                                 ush_size_t sz);

#endif // USHD_PUBLISH_FIFO_H
