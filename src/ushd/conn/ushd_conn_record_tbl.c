#include "pthread.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_type_pub.h"

#include "ush_comm_hello_msg.h"

#include "ushd_conn_record_tbl.h"
#include "ushd_publish_thread.h"

#define USHD_CONN_RECORD_TABLE_MAX_COUNT  (128)

typedef struct ushd_conn_record {
    ush_s32_t                    idx;   // equal to the offset in the table
    ush_bool_t                   valid; // 0 for invalid, 1 for valid
    ush_char_t                   name[USH_COMM_HELLO_MSG_NAME_LEN_MAX];
    ush_s32_t                    cert;
    ushd_publish_thread_t        publisher;
} * ushd_conn_record_t;

// ush_bool_t
// ushd_conn_record_is_valid(const ushd_conn_record_t record) {
//     if (!record) {
//         return 0;
//     }
//     return ushd_conn_table_is_record_valid(record->idx);
// }

// ush_ret_t
// ushd_conn_record_attr_set(ushd_conn_record_t         record,
//                           USHD_CONN_RECORD_ATTR_TYPE ty,
//                           const ush_vptr_t           ptr,
//                           ush_size_t                 sz) {
//     ush_assert(record && ptr);

//     return ushd_conn_table_set_record_attr(record->idx, ty, ptr, sz);
// }

// ush_s32_t
// ushd_conn_record_attr_get(const ushd_conn_record_t   record,
//                           USHD_CONN_RECORD_ATTR_TYPE ty,
//                           ush_vptr_t                 ptr,
//                           ush_size_t                 sz) {
//     ush_assert(record && ptr);

//     return ushd_conn_table_get_record_attr(record->idx, ty, ptr, sz);
// }


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    struct ushd_conn_record    records[USHD_CONN_RECORD_TABLE_MAX_COUNT];
    ush_s32_t                  next; // always increse by 1, NEVER descrese
    ush_s32_t                  max_idx;
} conn_tbl;

static conn_tbl tbl; // all 0 when init

ush_ret_t
ushd_conn_table_init() {
    //make idx 0 invalid
    tbl.records[0].idx       = 0xFFFFFFFF;
    tbl.records[0].valid     = 0;
    tbl.records[0].name[0]   = '\0';
    tbl.records[0].cert      = 0xFFFFFFFF;
    tbl.records[0].publisher = NULL;

    tbl.next = 1;
    tbl.max_idx  = USHD_CONN_RECORD_TABLE_MAX_COUNT - 1;

    return USH_RET_OK;
}

ush_s32_t
ushd_conn_table_add_record(const ush_char_t           *name,
                           ush_s32_t                   cert,
                           const ushd_publish_thread_t publisher) {
    ush_assert(name && (0xFFFFFFFF != cert) && publisher);
    if ((tbl.max_idx) < (tbl.next + 1)) {
        return -1;
    }

    ush_s32_t curr = tbl.next;

    tbl.records[curr].idx       = curr;
    tbl.records[curr].valid     = 1;
    strcpy(tbl.records[curr].name, name);
    tbl.records[curr].cert      = cert;
    tbl.records[curr].publisher = publisher;
    tbl.next                   += 1;

    return curr;
}

ush_u64_t
ushd_conn_table_get_record_ident(ush_s32_t idx) {
    if (idx < 0 || idx >= tbl.next) {
        return 0xFFFFFFFFFFFFFFFF;
    }

    return (((ush_u64_t)idx) << 32) | tbl.records[idx].cert;
}

// ush_bool_t
// ushd_conn_table_is_record_valid(ush_s32_t idx) {
//     const ushd_conn_record_t record = tbl.records[idx];

//     if (0 == idx || idx >= tbl.next || idx >= tbl.max || 0 == record.valid
//         || 0xFFFFFFFF == record.cert || NULL == record.publisher) {
//         return 0;
//     }

//     return 1;
// }
