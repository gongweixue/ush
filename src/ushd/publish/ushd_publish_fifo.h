#ifndef USHD_PUBLISH_FIFO_H
#define USHD_PUBLISH_FIFO_H

#include "ush_type_pub.h"
#include "ush_fifo_template.h"
#include "ushd_publish_fifo_msg.h"

#define USHD_PUBLISH_FIFO_ELEM_DATA_LEN   (USHD_PUBLISH_FIFO_MSG_MAX_SIZE)

USH_FIFO_DECL_CODE_GEN(ushd_publish);

#endif // USHD_PUBLISH_FIFO_H
