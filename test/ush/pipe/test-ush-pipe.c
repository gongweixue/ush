#include "case_ush_pipe_create.c"

#include "ushd_sched_thread.h"
#include "ushd_touch_thread.h"
#include "ush_time.h"

static void start_ushd() { // fake ushd
    ushd_sched_thread_start();
    ushd_touch_thread_start();
    ush_time_delay_ms(2000); // wait fake ushd
}

int main() {
    start_ushd();

    test_ush_pipe_create();
    return 0;
}
