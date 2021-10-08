#ifndef USHD_DISTH_FIFO_H
#define USHD_DIST_FIFO_H

#include "ush_type_pub.h"
#include "ush_fifo_template.h"
#include "ushd_dist_fifo_msg.h"

#define USHD_DIST_FIFO_ELEM_DATA_LEN   (USHD_DIST_FIFO_MSG_MAX_SIZE)

USH_FIFO_DECL_CODE_GEN(ushd_dist);

#endif // USHD_DIST_FIFO_H
