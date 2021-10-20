#ifndef USHD_CONN_REGLIST_H
#define USHD_CONN_REGLIST_H

#include "ush_type_pub.h"
#include "ush_sig_id.h"

ush_ret_t ushd_conn_reglist_init(void);

ush_ret_t ushd_conn_reglist_set_rcv(ush_connidx_t     idx,
                                    ush_sig_id_t      sigid,
                                    ush_pvoid_t       rcv);

#endif // USHD_CONN_REGLIST_H
