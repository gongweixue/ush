#ifndef USHD_DIST_PROC_SIG_H
#define USHD_DIST_PROC_SIG_H

#include "ush_type_pub.h"
#include "dist/ushd_dist_thread.h"
#include "dist/ushd_dist_fifo_msg.h"

ush_ret_t
ushd_dist_proc_sig_send_reg_ack(ushd_dist_thread_t thread,
                                const dist_fifo_msg_sig_reg_ack *msg);

#endif // USHD_DIST_PROC_SIG_H
