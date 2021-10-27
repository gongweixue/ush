#include "mqueue.h"
#include "pthread.h"
#include "string.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_comm_def.h"
#include "ush_log.h"

#include "ush_realm.h"

typedef struct ush_realm {
    mqd_t      mq;
    ush_char_t fullname[USH_COMM_CONN_FULL_NAME_LEN_MAX];
} * ush_realm_t;

ush_ret_t
ush_realm_alloc(ush_realm_t *pRealm, const ush_char_t *fullname) {
    ush_assert(pRealm);
    *pRealm = NULL;

    ush_realm_t tmp = (ush_realm_t)malloc(sizeof(struct ush_realm));
    if (!tmp) {
        ush_log(LOG_LVL_FATAL, "realm alloc failed");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_DETAIL, "realm heap mem allocate, addr %p", tmp);

    tmp->mq = USH_INVALID_MQD_VALUE;
    strcpy(tmp->fullname, fullname);

    *pRealm = tmp;

    return USH_RET_OK;
}

ush_ret_t
ush_realm_open(ush_realm_t realm) {
    ush_assert(realm);
    if (USH_INVALID_MQD_VALUE != realm->mq) { // maybe already opened
        ush_log(LOG_LVL_INFO, "realm already open");
        return USH_RET_OK;
    }

    ush_log(LOG_LVL_DETAIL, "try to open realm, addr %p", realm);

    realm->mq = mq_open(realm->fullname, O_WRONLY);
    if (USH_INVALID_MQD_VALUE == realm->mq) { // failed
        ush_log(LOG_LVL_INFO, "realm open done.");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}

static ush_ret_t
ush_realm_close(ush_realm_t realm) {
    ush_assert(realm);
    if (USH_INVALID_MQD_VALUE == realm->mq) {
        ush_log(LOG_LVL_INFO, "realm already closed");
        return USH_RET_OK;
    }

    ush_log(LOG_LVL_DETAIL, "close realm, addr %p", realm);
    if (0 != mq_close(realm->mq)) {
        ush_log(LOG_LVL_ERROR, "realm closed failed");
        return USH_RET_FAILED;
    }

    realm->mq = USH_INVALID_MQD_VALUE;

    return USH_RET_OK;
}

ush_ret_t
ush_realm_destroy_with_closing(ush_realm_t *pRealm) {
    ush_assert(pRealm);
    if (!(*pRealm)) {
        ush_log(LOG_LVL_INFO, "ush_realm_t NULL to be destroy");
        return USH_RET_OK;
    }

    // close it anyway, no matter if it has been opened.
    ush_realm_close(*pRealm);

    ush_log(LOG_LVL_DETAIL, "free mem of realm %p", *pRealm);
    free(*pRealm);
    pRealm = NULL;

    return USH_RET_OK;
}
