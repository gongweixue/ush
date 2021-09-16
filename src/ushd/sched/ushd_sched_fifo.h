#ifndef USHD_SCHED_FIFO_H
#define USHD_SCHED_FIFO_H

#include "ush_type_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ushd_sched_fifo_option{
    USHD_SCHED_FIFO_
} ushd_despatch_fifo_option_t;

ush_ret_t   ushd_sched_fifo_init(ush_u32_t len, ush_u32_t sz);
ush_char_t *ushd_sched_fifo_retain(ushd_despatch_fifo_option_t type);
ush_ret_t   ushd_sched_fifo_release(ushd_despatch_fifo_option_t type);


#ifdef __cplusplus
}
#endif

#endif // USHD_SCHED_FIFO_H
