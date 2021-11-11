#include "ush_log.h"

#include "ush_lstnr_proc_cmd.h"

void
ush_lstnr_proc_cmd_stop(ush_lstnr_t lstnr) {
    if (!lstnr) {
            return;
    }

    ush_lstnr_stop_close(&lstnr);
}
