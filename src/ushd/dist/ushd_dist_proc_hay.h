#ifndef USHD_DIST_PROC_HAY_H
#define USHD_DIST_PROC_HAY_H

#include "dist/ushd_dist_fifo_msg.h"
#include "dist/ushd_dist_thread.h"

void ushd_dist_proc_hay_send(ushd_dist_thread_t       thread,
                             const dist_fifo_msg_hay *msg);

#endif // USHD_DIST_PROC_HAY_H
