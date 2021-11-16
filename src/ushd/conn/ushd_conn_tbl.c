#include "string.h"

#include "ush_assert.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ush_comm_desc.h"
#include "ush_comm_tch_hello.h"

#include "ushd_conn_tbl.h"


typedef struct ushd_conn_item_s {
    ush_bool_t                   active; // USH_FALSE for inactive
    ush_char_t                   name[USH_COMM_CONN_FULL_NAME_LEN_MAX];
    ush_cert_t                   cert;
    ushd_dist_thread_t           dist;
    ushd_realm_thread_t          realm;
} * ushd_conn_item_t;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    struct ushd_conn_item_s    items[USH_CONN_IDX_MAX];
    ush_s32_t                  cursor;
} conn_tbl;

static conn_tbl tbl; // all 0 when init

static void move_cursor_to_next_empty_slot(void) {
    int pos = tbl.cursor;
    while (tbl.items[pos].active || USHD_INVALID_CONN_IDX_VALUE == pos) {
        pos = (pos + 1) % USH_CONN_IDX_MAX; // move to next
        if (pos == tbl.cursor) { // empty slot not found
            tbl.cursor = USHD_INVALID_CONN_IDX_VALUE;
            ushd_log(LOG_LVL_ERROR, "empty slot not found");
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
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].active  = USH_FALSE; // empty slot
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].name[0] = '\0'; // name empty
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].cert    = USH_INVALID_CERT_VALUE;
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].dist    = NULL;
    tbl.items[USHD_INVALID_CONN_IDX_VALUE].realm   = NULL;

    tbl.cursor = USHD_INVALID_CONN_IDX_VALUE;


    return USH_RET_OK;
}

ush_cert_t
ushd_conn_tbl_add(const ush_char_t         *name,
                  ush_cert_t                cert,
                  const ushd_dist_thread_t  dist,
                  const ushd_realm_thread_t realm) {
    ush_assert(name && (USH_INVALID_CERT_VALUE!=cert) && dist && realm);

    ush_connidx_t idx = USHD_INVALID_CONN_IDX_VALUE;

    move_cursor_to_next_empty_slot();
    if (USHD_INVALID_CONN_IDX_VALUE == tbl.cursor) {
        idx = USHD_INVALID_CONN_IDX_VALUE;
        ushd_log(LOG_LVL_ERROR, "add conn record failed");
        goto BAILED;
    }

    strncpy(tbl.items[tbl.cursor].name, name,
            sizeof(tbl.items[tbl.cursor].name));
    tbl.items[tbl.cursor].cert      = cert;
    tbl.items[tbl.cursor].dist      = dist;
    tbl.items[tbl.cursor].realm     = realm;
    tbl.items[tbl.cursor].active    = USH_TRUE;

    idx = tbl.cursor;

BAILED:
    return idx;
}

ush_ret_t
ushd_conn_tbl_remove(ush_connidx_t idx) {
    if (!ushd_conn_tbl_connidx_check(idx)) {
        return 1;
    }
    tbl.items[idx].active  = USH_FALSE; // deactive first, anyway
    tbl.items[idx].name[0] = '\0';
    tbl.items[idx].cert    = USH_INVALID_CERT_VALUE;
    tbl.items[idx].realm   = NULL;
    tbl.items[idx].dist    = NULL;

    return USH_RET_OK;
}

ush_bool_t
ushd_conn_tbl_connidx_check(ush_connidx_t idx) {
    return (idx > 0 && idx < USH_CONN_IDX_MAX);
}

ush_bool_t
ushd_conn_tbl_get_active_flg(ush_connidx_t idx) {
    if (!ushd_conn_tbl_connidx_check(idx)) {
        return USH_FALSE;
    }

    ush_bool_t ret = tbl.items[idx].active;

    return ret;
}

ush_cert_t
ushd_conn_tbl_get_cert(ush_s32_t idx) {
    ush_cert_t ret = USH_INVALID_CERT_VALUE;

    if (!ushd_conn_tbl_connidx_check(idx)) {
        ret = USH_INVALID_CERT_VALUE;
    } else  if (USH_FALSE == tbl.items[idx].active) {
        ret = USH_INVALID_CERT_VALUE;
    } else {
        ret = tbl.items[idx].cert;
    }

    return ret;
}

ushd_dist_thread_t
ushd_conn_tbl_get_dist(ush_connidx_t idx) {
    ushd_dist_thread_t ret = NULL;

    if (!ushd_conn_tbl_connidx_check(idx)) {
        ushd_log(LOG_LVL_ERROR, "out of bound");
    } else if (USH_FALSE == tbl.items[idx].active) {
        ushd_log(LOG_LVL_ERROR, "record not active");
    } else {
        ret = tbl.items[idx].dist;
    }

    return ret;
}
