#ifndef USHD_SCHED_FIFO_H
#define USHD_SCHED_FIFO_H

#include "ush_type_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum USHD_SCHED_FIFO_OPTION{
    USHD_SCHED_FIFO_
} USHD_SCHED_FIFO_OPTION;

ush_ret_t   ushd_sched_fifo_init(ush_u32_t len, ush_u32_t sz);
ush_char_t *ushd_sched_fifo_retain(USHD_SCHED_FIFO_OPTION type);
ush_ret_t   ushd_sched_fifo_release(USHD_SCHED_FIFO_OPTION type);


#ifdef __cplusplus
}
#endif

#endif // USHD_SCHED_FIFO_H
