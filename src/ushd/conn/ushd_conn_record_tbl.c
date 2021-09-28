#include "pthread.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_def_pub.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ush_comm_hello_msg.h"

#include "ushd_conn_record_tbl.h"
#include "ushd_publish_thread.h"

#define RECORD_TABLE_MAX_COUNT  (128)

typedef struct ushd_conn_record {
    ush_s32_t                    idx;   // equal to the offset in the table
    ush_bool_t                   valid; // 0 for invalid, 1 for valid
    ush_char_t                   name[USH_COMM_CONN_NAME_LEN_MAX];
    ush_s32_t                    cert;
    ushd_publish_thread_t        publish;
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

#define CONN_INVALID_IDX (0)

typedef pthread_mutex_t * conn_tbl_cs_guard_t;

typedef struct {
    struct ushd_conn_record    records[RECORD_TABLE_MAX_COUNT];
    ush_s32_t                  cursor;
    conn_tbl_cs_guard_t        cs;
} conn_tbl;

static conn_tbl tbl; // all 0 when init

static void conn_tbl_cs_entry(conn_tbl_cs_guard_t cs) {
    pthread_mutex_lock(cs);
}

static void conn_tbl_cs_exit(conn_tbl_cs_guard_t cs) {
    pthread_mutex_unlock(cs);
}

static void move_cursor_to_next_empty_record() {
    int pos = tbl.cursor;
    while (tbl.records[pos].valid || CONN_INVALID_IDX == pos) {
        pos = (pos + 1) % RECORD_TABLE_MAX_COUNT; // move to next
        if (pos == tbl.cursor) { // empty slot not found
            tbl.cursor = CONN_INVALID_IDX;
            goto BAILED;
        }
    }

    if (!tbl.records[pos].valid) {
            tbl.cursor = pos;
    } else {
        tbl.cursor = CONN_INVALID_IDX;
    }

BAILED:
    return;
}

ush_ret_t
ushd_conn_table_init() {
    //make idx 0 invalid
    tbl.records[0].idx       = 0;
    tbl.records[0].valid     = 0;          // 0 means empty slot
    tbl.records[0].name[0]   = '\0';
    tbl.records[0].cert      = USH_INVALID_CERT_VALUE_DEFAULT;
    tbl.records[0].publish   = NULL;

    tbl.cursor = CONN_INVALID_IDX;

    pthread_mutex_init(&tbl.mutex);

    ushd_log(LOG_LVL_DETAIL, "conn table init finished");

    return USH_RET_OK;
}

ush_s32_t
ushd_conn_table_add_record(const ush_char_t           *name,
                           ush_s32_t                   cert,
                           const ushd_publish_thread_t publish) {
    ush_assert(name && (USH_INVALID_CERT_VALUE_DEFAULT != cert) && publish);

    conn_tbl_cs_entry(tbl.cs);

    move_cursor_to_next_empty_record();
    if (CONN_INVALID_IDX == tbl.cursor) {
        return CONN_INVALID_IDX;
    }

    tbl.records[tbl.cursor].idx       = tbl.cursor;
    tbl.records[tbl.cursor].valid     = 1;
    strcpy(tbl.records[tbl.cursor].name, name);
    tbl.records[tbl.cursor].cert      = cert;
    tbl.records[tbl.cursor].publish   = publish;

    conn_tbl_cs_exit(tbl.cs);

    ushd_log(LOG_LVL_INFO, "a new conn record into the table");

    return tbl.cursor;
}

ush_s32_t
ushd_conn_table_get_record_cert(ush_s32_t idx) {
    ush_s32_t ret = USH_INVALID_CERT_VALUE_DEFAULT;
    conn_tbl_cs_entry(tbl.cs);

    if (idx <= 0 && idx >= RECORD_TABLE_MAX_COUNT) {
        ret = USH_INVALID_CERT_VALUE_DEFAULT;
    } else  if (0 == tbl.records[idx].valid) {
        ret = USH_INVALID_CERT_VALUE_DEFAULT;
    } else {
        ret = tbl.records[idx].cert;
    }

    conn_tbl_cs_exit(tbl.cs);
    return ret;
}
