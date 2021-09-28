#include "pthread.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_def_pub.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ush_comm_hello_msg.h"

#include "ushd_conn_record_tbl.h"
#include "ushd_publish_thread.h"

#define USHD_CONN_RECORD_TABLE_MAX_COUNT  (128)

typedef struct ushd_conn_record {
    ush_s32_t                    idx;   // equal to the offset in the table
    ush_bool_t                   valid; // 0 for invalid, 1 for valid
    ush_char_t                   name[USH_COMM_LISTENER_Q_NAME_LEN_MAX];
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
    ush_s32_t                  cursor;
} conn_tbl;

static conn_tbl tbl; // all 0 when init

static void move_cursor_to_next_invalid() {
    while (tbl.records[tbl.cursor].valid || 0 == tbl.cursor) {
        static int counter = 0;
        counter++;

        if (USHD_CONN_RECORD_TABLE_MAX_COUNT <= counter) { // no more for now
            tbl.cursor = 0;
            break;
        } else {
            tbl.cursor = (tbl.cursor + 1) % USHD_CONN_RECORD_TABLE_MAX_COUNT;
        }
    }
    return;
}

ush_ret_t
ushd_conn_table_init() {
    //make idx 0 invalid
    tbl.records[0].idx       = 0;
    tbl.records[0].valid     = 0;          // 0 means empty slot
    tbl.records[0].name[0]   = '\0';
    tbl.records[0].cert      = USH_INVALID_CERT_VALUE_DEFAULT;
    tbl.records[0].publisher = NULL;

    tbl.cursor = 0;

    ushd_log(LOG_LVL_DETAIL, "conn table init finished");

    return USH_RET_OK;
}

ush_s32_t
ushd_conn_table_add_record(const ush_char_t           *name,
                           ush_s32_t                   cert,
                           const ushd_publish_thread_t publisher) {
    ush_assert(name && (USH_INVALID_CERT_VALUE_DEFAULT != cert) && publisher);

    move_cursor_to_next_invalid();
    if (0 == tbl.cursor) {
        return -1;
    }

    tbl.records[tbl.cursor].idx       = tbl.cursor;
    tbl.records[tbl.cursor].valid     = 1;
    strcpy(tbl.records[tbl.cursor].name, name);
    tbl.records[tbl.cursor].cert      = cert;
    tbl.records[tbl.cursor].publisher = publisher;

    ushd_log(LOG_LVL_INFO, "a new conn record into the table");

    return tbl.cursor;
}

ush_s32_t
ushd_conn_table_get_record_cert(ush_s32_t idx) {
    if (idx <= 0 || 0 == tbl.records[idx].valid) {
        return USH_INVALID_CERT_VALUE_DEFAULT;
    }

    return tbl.records[idx].cert;
}

// ush_bool_t
// ushd_conn_table_is_record_valid(ush_s32_t idx) {
//     const ushd_conn_record_t record = tbl.records[idx];

//     if (0 == idx || idx > tbl.max_idx || 0 == record.valid
//         || INVALID_TID == record.cert || NULL == record.publisher) {
//         return 0;
//     }

//     return 1;
// }
