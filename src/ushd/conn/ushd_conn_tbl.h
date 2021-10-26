#ifndef USHD_CONN_TBL_H
#define USHD_CONN_TBL_H

#include "ush_type_pub.h"

#include "ushd_conn_reglist.h"

#include "dist/ushd_dist_thread.h"
#include "realm/ushd_realm_thread.h"

////////////////////////////////////////////////////////////////////////////////
//////////////////             conn table               /////////////////
////////////////////////////////////////////////////////////////////////////////
ush_ret_t ushd_conn_tbl_init(void);

// return -1 means failed
ush_cert_t ushd_conn_tbl_add(const ush_char_t           *name,
                             ush_cert_t                  cert,
                             const ushd_dist_thread_t    dist,
                             const ushd_realm_thread_t   realm);

ush_ret_t  ushd_conn_tbl_remove(ush_connidx_t idx);

ush_bool_t ushd_conn_tbl_check_connidx(ush_connidx_t idx);

ush_bool_t ushd_conn_tbl_get_valid_flg(ush_connidx_t idx);

ush_cert_t ushd_conn_tbl_get_cert(ush_connidx_t idx);

ushd_dist_thread_t ushd_conn_tbl_get_dist(ush_connidx_t idx);

#endif // USHD_CONN_TBL_H
