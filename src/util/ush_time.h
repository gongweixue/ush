#ifndef USH_TIME_H
#define USH_TIME_H

#include "unistd.h"
#include "ush_type_pub.h"

#define USH_TIME_EXTRA_NSEC_TO_WAIT     (500 * 100 * 100) // extra 500ms

#define USH_TIME_NSEC_PER_SEC               (1000 * 1000 * 1000)

static inline void ush_time_delay_ms(ush_u32_t time) {
    if (0 == time) {
        return;
    }
    if (time >= 1000) { // use sleep to solve time greate-equal to 1s.
        sleep(time / 1000);
    }

    usleep((time % 1000) * 1000); // ms->us
}

#endif // USH_TIME_H
