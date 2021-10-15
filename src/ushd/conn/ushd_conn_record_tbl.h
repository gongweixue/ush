#ifndef USHD_CONN_RECORD_TBL_H
#define USHD_CONN_RECORD_TBL_H

#include "ush_type_pub.h"

#include "dist/ushd_dist_thread.h"
#include "reg/ushd_reg_list.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////     conn record                   /////////////////////
typedef struct ushd_conn_record * ushd_conn_record_t;


////////////////////////////////////////////////////////////////////////////////
//////////////////             conn record table               /////////////////
////////////////////////////////////////////////////////////////////////////////
ush_ret_t ushd_conn_table_init();

// return -1 means failed
ush_s32_t ushd_conn_table_add_record(const ush_char_t           *name,
                                     ush_s32_t                   cert,
                                     const ushd_dist_thread_t    dist);

ush_bool_t ushd_conn_table_get_record_valid(ush_s32_t idx);
ush_s32_t  ushd_conn_table_get_record_cert(ush_s32_t idx);

ushd_reg_list_t ushd_conn_table_get_record_reglist(ush_s32_t idx);

ushd_dist_thread_t ushd_conn_table_get_record_dist(ush_s32_t idx);

ush_ret_t
ushd_conn_table_set_cb_rcv(ush_s32_t idx, ush_sig_id_t sigid, ush_pvoid_t rcv);

#endif // USHD_CONN_RECORD_TBL_H
