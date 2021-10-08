#ifndef USHD_CONN_RECORD_TBL_H
#define USHD_CONN_RECORD_TBL_H

#include "ush_type_pub.h"

#include "ushd_dist_thread.h"

typedef enum {
    USHD_CONN_RECORD_ATTR_NAME,
    USHD_CONN_RECORD_ATTR_CERT,
    USHD_CONN_RECORD_ATTR_IDX,
    USHD_CONN_RECORD_ATTR_VALID
} USHD_CONN_RECORD_ATTR_TYPE;


////////////////////////////////////////////////////////////////////////////////
////////////////////////     conn record                   /////////////////////

typedef struct ushd_conn_record * ushd_conn_record_t;

// ush_bool_t ushd_conn_record_is_valid(const ushd_conn_record_t record);

// ush_ret_t ushd_conn_record_attr_set(ushd_conn_record_t         record,
//                                     USHD_CONN_RECORD_ATTR_TYPE ty,
//                                     const ush_vptr_t           ptr,
//                                     ush_size_t                 sz);

// // return real size of value to get, -1 means fail
// // sz means the max size of ptr, must be larger-equal than the real size to get
// ush_s32_t ushd_conn_record_attr_get(const ushd_conn_record_t   record,
//                                     USHD_CONN_RECORD_ATTR_TYPE ty,
//                                     ush_vptr_t                 ptr,
//                                     ush_size_t                 sz);

////////////////////////////////////////////////////////////////////////////////
//////////////////             conn record table               /////////////////
////////////////////////////////////////////////////////////////////////////////

ush_ret_t ushd_conn_table_init();

// return -1 means failed
ush_s32_t ushd_conn_table_add_record(const ush_char_t           *name,
                                     ush_s32_t                   cert,
                                     const ushd_dist_thread_t    dist);

ush_s32_t ushd_conn_table_get_record_cert(ush_s32_t idx);

// ush_bool_t ushd_conn_table_is_record_valid(ush_s32_t idx);

// ush_ret_t ushd_conn_table_set_record_attr(ush_s32_t                  idx,
//                                           USHD_CONN_RECORD_ATTR_TYPE ty,
//                                           const ush_vptr_t           ptr,
//                                           ush_size_t                 sz);

// // return real size of value to get, -1 means fail
// // sz means the max size of ptr, must be larger-equal than the real size to get
// ush_s32_t ushd_conn_table_get_record_attr(ush_s32_t                  idx,
//                                           USHD_CONN_RECORD_ATTR_TYPE ty,
//                                           ush_vptr_t                 ptr,
//                                           ush_size_t                 sz);


#endif // USHD_CONN_RECORD_TBL_H
