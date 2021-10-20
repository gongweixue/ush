#ifndef USHD_CONN_TBL_H
#define USHD_CONN_TBL_H

#include "ush_type_pub.h"

#include "ushd_conn_reglist.h"

#include "dist/ushd_dist_thread.h"

////////////////////////////////////////////////////////////////////////////////
//////////////////             conn table               /////////////////
////////////////////////////////////////////////////////////////////////////////
ush_ret_t ushd_conn_tbl_init();

// return -1 means failed
ush_cert_t ushd_conn_tbl_add(const ush_char_t           *name,
                             ush_cert_t                  cert,
                             const ushd_dist_thread_t    dist);

ush_bool_t ush_conn_tbl_connidx_valid(ush_connidx_t idx);

ush_bool_t ushd_conn_tbl_get_valid(ush_connidx_t idx);

ush_cert_t ushd_conn_tbl_get_cert(ush_connidx_t idx);

ushd_dist_thread_t ushd_conn_tbl_get_dist(ush_connidx_t idx);

#endif // USHD_CONN_TBL_H