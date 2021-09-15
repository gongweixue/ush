#ifndef USH_TIME_H
#define USH_TIME_H

#include "unistd.h"
#include "ush_type_pub.h"

static inline void ush_time_delay_ms(ush_u32_t time) {
    usleep(time * 1000); // ms->us
}

#endif // USH_TIME_H
