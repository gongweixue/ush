#include "pthread.h"
#include "stdlib.h"
#include "string.h"

#include "ush_type_pub.h"

#include "ush_sig_enum.h"

#include "ushd_reg_list.h"

typedef enum {
    INTERVAL_BASE_NONE,
    INTERVAL_BASE_10ms,
    INTERVAL_BASE_100ms,
    INTERVAL_BASE_1s,
} INTERVAL_BASE;

typedef enum {
    INTERVAL_FCTR_NONE,
    INTERVAL_FCTR_x1,
    INTERVAL_FCTR_x2,
    INTERVAL_FCTR_x5,
} INTERVAL_FCTR;

typedef struct signal_record {
    ush_pvoid_t    cb_rcv; // null for 'not subscribe'
    INTERVAL_BASE  intervalbase;
    INTERVAL_FCTR  intervalfactor;
} signal_record;


typedef struct ushd_reg_list {
    pthread_mutex_t  mutex;
    signal_record elems[USH_SIG_ID_MAX];
} * ushd_reg_list_t;


ushd_reg_list_t
ushd_reg_list_create() {
    ushd_reg_list_t ret = (ushd_reg_list_t)malloc(sizeof(struct ushd_reg_list));
    if (!ret) {
        return NULL;
    }
    memset(ret->elems, 0, sizeof(ret->elems));
    if (0 != pthread_mutex_init(&(ret->mutex), NULL)) {
        free(ret);
        ret = NULL;
    }

    return ret;
}

static inline void ushd_reg_list_cs_enter(ushd_reg_list_t reglist) {
    pthread_mutex_lock(&reglist->mutex);
}
static inline void ushd_reg_list_cs_exit(ushd_reg_list_t reglist) {
    pthread_mutex_unlock(&reglist->mutex);
}


ush_ret_t ushd_reg_list_set_cb_rcv(ushd_reg_list_t reglist,
                                   ush_sig_id_t    sigid,
                                   ush_pvoid_t     rcv) {
    if (!reglist) {
        return USH_RET_FAILED;
    }

    if (!ush_sig_id_valid(sigid)) {
        return USH_RET_FAILED;
    }

    ushd_reg_list_cs_enter(reglist);
    reglist->elems[sigid].cb_rcv = rcv;
    ushd_reg_list_cs_exit(reglist);

    return USH_RET_OK;
}
