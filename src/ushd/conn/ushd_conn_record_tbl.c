#include "pthread.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ush_comm_desc.h"
#include "tch/ush_comm_tch_hello.h"

#include "ushd_conn_record_tbl.h"

#define RECORD_TABLE_MAX_COUNT  (128)

typedef struct ushd_conn_record {
    ush_bool_t                   valid; // 0 for invalid, 1 for valid
    ush_char_t                   name[USH_COMM_CONN_NAME_LEN_MAX];
    ush_s32_t                    cert;
    ushd_dist_thread_t           dist;
    ushd_reg_list_t              regs;
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
//                           const ush_pvoid_t           ptr,
//                           ush_size_t                 sz) {
//     ush_assert(record && ptr);

//     return ushd_conn_table_set_record_attr(record->idx, ty, ptr, sz);
// }

// ush_s32_t
// ushd_conn_record_attr_get(const ushd_conn_record_t   record,
//                           USHD_CONN_RECORD_ATTR_TYPE ty,
//                           ush_pvoid_t                 ptr,
//                           ush_size_t                 sz) {
//     ush_assert(record && ptr);

//     return ushd_conn_table_get_record_attr(record->idx, ty, ptr, sz);
// }


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef pthread_mutex_t conn_tbl_cs_guard_t;

typedef struct {
    struct ushd_conn_record    records[RECORD_TABLE_MAX_COUNT];
    ush_s32_t                  cursor;
    conn_tbl_cs_guard_t        cs;
} conn_tbl;

static conn_tbl tbl; // all 0 when init

static void conn_tbl_cs_entry() {
    pthread_mutex_lock(&tbl.cs);
}

static void conn_tbl_cs_exit() {
    pthread_mutex_unlock(&tbl.cs);
}

static void move_cursor_to_next_empty_record() {
    int pos = tbl.cursor;
    while (tbl.records[pos].valid || USHD_INVALID_CONN_IDX_VALUE == pos) {
        pos = (pos + 1) % RECORD_TABLE_MAX_COUNT; // move to next
        if (pos == tbl.cursor) { // empty slot not found
            tbl.cursor = USHD_INVALID_CONN_IDX_VALUE;
            goto BAILED;
        }
    }

    if (!tbl.records[pos].valid) {
            tbl.cursor = pos;
    } else {
        tbl.cursor = USHD_INVALID_CONN_IDX_VALUE;
    }

BAILED:
    return;
}

ush_ret_t
ushd_conn_table_init() {
    //make idx 0 invalid
    tbl.records[0].valid     = 0;          // 0 means empty slot
    tbl.records[0].name[0]   = '\0';
    tbl.records[0].cert      = USH_INVALID_CERT_VALUE;
    tbl.records[0].dist      = NULL;
    tbl.records[0].regs      = NULL;

    tbl.cursor = USHD_INVALID_CONN_IDX_VALUE;

    pthread_mutex_init(&tbl.cs, NULL);

    ushd_log(LOG_LVL_DETAIL, "conn table init finished");

    return USH_RET_OK;
}

ush_s32_t
ushd_conn_table_add_record(const ush_char_t           *name,
                           ush_s32_t                   cert,
                           const ushd_dist_thread_t    dist) {
    ush_assert(name && (USH_INVALID_CERT_VALUE!=cert) && dist);

    ush_s32_t idx = USHD_INVALID_CONN_IDX_VALUE;

    conn_tbl_cs_entry();

    move_cursor_to_next_empty_record();
    if (USHD_INVALID_CONN_IDX_VALUE == tbl.cursor) {
        idx = USHD_INVALID_CONN_IDX_VALUE;
        goto BAILED;
    }

    ushd_reg_list_t regs = ushd_reg_list_create();
    if (!regs) {
        idx = USHD_INVALID_CONN_IDX_VALUE;
        goto BAILED;
    }
    tbl.records[tbl.cursor].regs      = regs;

    tbl.records[tbl.cursor].valid     = 1;
    strcpy(tbl.records[tbl.cursor].name, name);
    tbl.records[tbl.cursor].cert      = cert;
    tbl.records[tbl.cursor].dist      = dist;

    idx = tbl.cursor;

BAILED:
    conn_tbl_cs_exit();

    ushd_log(LOG_LVL_INFO, "a new conn record into the table");

    return idx;
}

ush_bool_t
ushd_conn_table_get_record_valid(ush_s32_t idx) {
    if (idx <= 0 || idx >= RECORD_TABLE_MAX_COUNT) {
        return 0;
    }

    conn_tbl_cs_entry();
    ush_bool_t ret = tbl.records[idx].valid;
    conn_tbl_cs_exit();
    return ret;
}

ush_s32_t
ushd_conn_table_get_record_cert(ush_s32_t idx) {
    ush_s32_t ret = USH_INVALID_CERT_VALUE;

    conn_tbl_cs_entry();

    if (idx <= 0 && idx >= RECORD_TABLE_MAX_COUNT) {
        ret = USH_INVALID_CERT_VALUE;
    } else  if (0 == tbl.records[idx].valid) {
        ret = USH_INVALID_CERT_VALUE;
    } else {
        ret = tbl.records[idx].cert;
    }

    conn_tbl_cs_exit();
    return ret;
}

ushd_reg_list_t
ushd_conn_table_get_record_reglist(ush_s32_t idx) {
    ushd_reg_list_t ret = NULL;

    conn_tbl_cs_entry();

    if (idx <= 0 && idx >= RECORD_TABLE_MAX_COUNT) {
        ushd_log(LOG_LVL_ERROR, "out of bound");
    } else if (0 == tbl.records[idx].valid) {
        ushd_log(LOG_LVL_ERROR, "record not valid");
    } else {
        ret = tbl.records[idx].regs;
    }

    conn_tbl_cs_exit();

    return ret;
}

ushd_dist_thread_t
ushd_conn_table_get_record_dist(ush_s32_t idx) {
    ushd_dist_thread_t ret = NULL;

    conn_tbl_cs_entry();

    if (idx <= 0 && idx >= RECORD_TABLE_MAX_COUNT) {
        ushd_log(LOG_LVL_ERROR, "out of bound");
    } else if (0 == tbl.records[idx].valid) {
        ushd_log(LOG_LVL_ERROR, "record not valid");
    } else {
        ret = tbl.records[idx].dist;
    }

    conn_tbl_cs_exit();

    return ret;
}

ush_ret_t
ushd_conn_table_set_cb_rcv(ush_s32_t idx, ush_sig_id_t sigid, ush_pvoid_t rcv) {
    ushd_reg_list_t reglist = ushd_conn_table_get_record_reglist(idx);
    return ushd_reg_list_set_cb_rcv(reglist, sigid, rcv);
}
