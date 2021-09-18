#ifndef USHD_SCHED_FIFO_H
#define USHD_SCHED_FIFO_H

#include "ush_type_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum USHD_SCHED_FIFO_OPTION{
    USHD_SCHED_FIFO_EMPTY,
    USHD_SCHED_FIFO_FULL
} USHD_SCHED_FIFO_OPTION;


ush_ret_t   ushd_sched_fifo_init();

// always return USH_COMM_TOUCH_Q_MSG_MAX_LEN big's buffer.
ush_char_t *ushd_sched_fifo_retain(USHD_SCHED_FIFO_OPTION type);

ush_ret_t ushd_sched_fifo_release(ush_char_t *buf, USHD_SCHED_FIFO_OPTION opt);


#ifdef __cplusplus
}
#endif

#endif // USHD_SCHED_FIFO_H
