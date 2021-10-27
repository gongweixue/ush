#include "pthread.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ush_comm_desc.h"
#include "tch/ush_comm_tch_hello.h"

#include "ushd_conn_tbl.h"


typedef struct ushd_conn_item {
    ush_bool_t                   active; // 0 for inactive, 1 for active
    ush_char_t                   name[USH_COMM_CONN_FULL_NAME_LEN_MAX];
    ush_cert_t                   cert;
    ushd_dist_thread_t           dist;
    ushd_realm_thread_t          realm;
} * ushd_conn_item_t;

// ush_bool_t
// ushd_conn_record_is_valid(const ushd_conn_item_t record) {
//     if (!record) {
//         return 0;
//     }
//     return ushd_conn_tbl_is_record_valid(record->idx);
// }

// ush_ret_t
// ushd_conn_record_attr_set(ushd_conn_item_t         record,
//                           USHD_CONN_RECORD_ATTR_TYPE ty,
//                           const ush_pvoid_t           ptr,
//                           ush_size_t                 sz) {
//     ush_assert(record && ptr);

//     return ushd_conn_tbl_set_record_attr(record->idx, ty, ptr, sz);
// }

// ush_s32_t
// ushd_conn_record_attr_get(const ushd_conn_item_t   record,
//                           USHD_CONN_RECORD_ATTR_TYPE ty,
//                           ush_pvoid_t                 ptr,
//                           ush_size_t                 sz) {
//     ush_assert(record && ptr);

//     return ushd_conn_tbl_get_record_attr(record->idx, ty, ptr, sz);
// }


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef pthread_mutex_t conn_tbl_cs_guard_t;

typedef struct {
    struct ushd_conn_item      items[USH_CONN_IDX_MAX];
    ush_s32_t                  cursor;
    conn_tbl_cs_guard_t        cs;
} conn_tbl;

static conn_tbl tbl; // all 0 when init

static void conn_tbl_cs_entry(void) {
    pthread_mutex_lock(&tbl.cs);
}

static void conn_tbl_cs_exit(void) {
    pthread_mutex_unlock(&tbl.cs);
}

static void move_cursor_to_next_empty_record(void) {
    int pos = tbl.cursor;
    while (tbl.items[pos].active || USHD_INVALID_CONN_IDX_VALUE == pos) {
        pos = (pos + 1) % USH_CONN_IDX_MAX; // move to next
        if (pos == tbl.cursor) { // empty slot not found
            tbl.cursor = USHD_INVALID_CONN_IDX_VALUE;
            goto BAILED;
        }
    }

    if (!tbl.items[pos].active) {
            tbl.cursor = pos;
    } else {
        tbl.cursor = USHD_INVALID_CONN_IDX_VALUE;
    }

BAILED:
    return;
}

ush_ret_t
ushd_conn_tbl_init(void) {
    //make idx 0 inactive
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].active  = 0;   // 0 means empty slot
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].name[0] = '\0'; // name empty
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].cert    = USH_INVALID_CERT_VALUE;
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].dist    = NULL;
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].realm   = NULL;

    tbl.cursor = USHD_INVALID_CONN_IDX_VALUE;

    pthread_mutex_init(&tbl.cs, NULL);

    ushd_log(LOG_LVL_DETAIL, "conn table init finished");

    return USH_RET_OK;
}

ush_cert_t
ushd_conn_tbl_add(const ush_char_t         *name,
                  ush_cert_t                cert,
                  const ushd_dist_thread_t  dist,
                  const ushd_realm_thread_t realm) {
    ush_assert(name && (USH_INVALID_CERT_VALUE!=cert) && dist && realm);

    ush_connidx_t idx = USHD_INVALID_CONN_IDX_VALUE;

    conn_tbl_cs_entry();

    move_cursor_to_next_empty_record();
    if (USHD_INVALID_CONN_IDX_VALUE == tbl.cursor) {
        idx = USHD_INVALID_CONN_IDX_VALUE;
        goto BAILED;
    }

    strcpy(tbl.items[tbl.cursor].name, name);
    tbl.items[tbl.cursor].cert      = cert;
    tbl.items[tbl.cursor].dist      = dist;
    tbl.items[tbl.cursor].realm     = realm;
    tbl.items[tbl.cursor].active    = 1;

    idx = tbl.cursor;

BAILED:
    conn_tbl_cs_exit();

    ushd_log(LOG_LVL_INFO, "a new conn record into the table");

    return idx;
}

ush_ret_t  ushd_conn_tbl_remove(ush_connidx_t idx) {
    if (!ushd_conn_tbl_connidx_check(idx)) {
        return 1;
    }

    conn_tbl_cs_entry();
    tbl.items[idx].active = 0;
    conn_tbl_cs_exit();
    return 1;
}

ush_bool_t ushd_conn_tbl_connidx_check(ush_connidx_t idx) {
    return (idx > 0 && idx < USH_CONN_IDX_MAX);
}

ush_bool_t
ushd_conn_tbl_get_active_flg(ush_connidx_t idx) {
    if (!ushd_conn_tbl_connidx_check(idx)) {
        return 0;
    }

    conn_tbl_cs_entry();
    ush_bool_t ret = tbl.items[idx].active;
    conn_tbl_cs_exit();
    return ret;
}

ush_cert_t
ushd_conn_tbl_get_cert(ush_s32_t idx) {
    ush_cert_t ret = USH_INVALID_CERT_VALUE;

    conn_tbl_cs_entry();

    if (!ushd_conn_tbl_connidx_check(idx)) {
        ret = USH_INVALID_CERT_VALUE;
    } else  if (0 == tbl.items[idx].active) {
        ret = USH_INVALID_CERT_VALUE;
    } else {
        ret = tbl.items[idx].cert;
    }

    conn_tbl_cs_exit();
    return ret;
}

ushd_dist_thread_t
ushd_conn_tbl_get_dist(ush_connidx_t idx) {
    ushd_dist_thread_t ret = NULL;

    conn_tbl_cs_entry();

    if (!ushd_conn_tbl_connidx_check(idx)) {
        ushd_log(LOG_LVL_ERROR, "out of bound");
    } else if (0 == tbl.items[idx].active) {
        ushd_log(LOG_LVL_ERROR, "record not active");
    } else {
        ret = tbl.items[idx].dist;
    }

    conn_tbl_cs_exit();

    return ret;
}
