#ifndef USHD_SCHED_FIFO_H
#define USHD_SCHED_FIFO_H

#include "ush_type_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// The sched fifo is for touch thread pushing and sched thread poping.
//
// Consider the overhead of copying in and out, the single queue is obviously
// not enough, because that the current thread may cause the blocking of
// all other threads in some cases(local thread may enter the critical section
// and the touch thread will be blocked by it to copy data into the buf).
//
// And the mqueue len is limited, so we used double queue to avoid the
// twice-copy(mqueue->schedfifo, and schedfifo->local buf), and avoid the
// blocking(the fifo is too big for the mqueue len).
//
// The overhead is elimated by using in-placed data-deal(buffer retain/release).
////////////////////////////////////////////////////////////////////////////////

typedef enum USHD_SCHED_FIFO_OPTION{
    USHD_SCHED_FIFO_EMPTY,
    USHD_SCHED_FIFO_FULL
} USHD_SCHED_FIFO_OPTION;


ush_ret_t   ushd_sched_fifo_init();

// always return USH_COMM_TOUCH_Q_MSG_MAX_LEN big's buffer.
ush_char_t *ushd_sched_fifo_retain(USHD_SCHED_FIFO_OPTION opt);

ush_ret_t ushd_sched_fifo_release(ush_char_t *buf, USHD_SCHED_FIFO_OPTION opt);


#ifdef __cplusplus
}
#endif

#endif // USHD_SCHED_FIFO_H
