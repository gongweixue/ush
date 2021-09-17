#include "ush_type_pub.h"

#include "ush_time.h"

#include "ushd_sched_fifo.h"


ush_ret_t   ushd_sched_fifo_init(ush_u32_t len, ush_u32_t sz) {
    ush_time_delay_ms(500);
    return USH_RET_FAILED;
}

// always return USH_COMM_TOUCH_Q_MSG_MAX_LEN big buffer.
ush_char_t *ushd_sched_fifo_retain(USHD_SCHED_FIFO_OPTION type) {
    ush_time_delay_ms(500);
    return NULL;
}

ush_ret_t ushd_sched_fifo_release(ush_char_t *buf, USHD_SCHED_FIFO_OPTION opt) {
    ush_time_delay_ms(500);
    return USH_RET_FAILED;
}
