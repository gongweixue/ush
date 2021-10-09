#ifndef USHD_CONN_RECORD_TBL_H
#define USHD_CONN_RECORD_TBL_H

#include "ush_type_pub.h"

#include "dist/ushd_dist_thread.h"
#include "reg/ushd_reg_list.h"

// typedef enum {
//     USHD_CONN_RECORD_ATTR_NAME,
//     USHD_CONN_RECORD_ATTR_CERT,
//     USHD_CONN_RECORD_ATTR_IDX,
//     USHD_CONN_RECORD_ATTR_VALID
// } USHD_CONN_RECORD_ATTR_TYPE;


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

ush_s32_t ushd_conn_table_get_record_cert(ush_s32_t idx);

ushd_reg_list_t ushd_conn_table_get_record_reglist(ush_s32_t idx);


#endif // USHD_CONN_RECORD_TBL_H
