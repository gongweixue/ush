#ifndef USHD_CONN_REGLIST_H
#define USHD_CONN_REGLIST_H

#include "ush_type_pub.h"
#include "ush_sig_id.h"

ush_ret_t ushd_conn_reglist_init(void);

ush_ret_t ushd_conn_reglist_set_rcv(ush_connidx_t     idx,
                                    ush_sig_id_t      sigid,
                                    ush_pvoid_t       rcv);

// compare and set if different, return USH_RET_OK if set success
ush_ret_t ushd_conn_reglist_cas(ush_sig_id_t sigid, ush_sig_val_t val);

typedef void (*notify_func_t)(
    ush_connidx_t,
    ush_sig_id_t,
    ush_sig_val_t,
    ush_pvoid_t,
    ush_u32_t);

ush_ret_t ushd_conn_reglist_notify(ush_sig_id_t sigid, notify_func_t func);

#endif // USHD_CONN_REGLIST_H
