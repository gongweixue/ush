#ifndef USHD_SCHED_FIFO_H
#define USHD_SCHED_FIFO_H

#include "ush_type_pub.h"

#include "ush_fifo_template.h"

#define USHD_SCHED_FIFO_ELEM_DATA_LEN   (USH_COMM_TCH_Q_MSG_MAX_LEN)

USH_FIFO_DECL_CODE_GEN(ushd_sched);

ushd_sched_fifo_t ushd_sched_fifo_singleton(void);

#endif // USHD_SCHED_FIFO_H
