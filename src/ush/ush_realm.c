#include "mqueue.h"
#include "pthread.h"
#include "string.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_comm_def.h"
#include "ush_log.h"

#include "ush_realm.h"
#include "realm/sig/ush_comm_realm_sig.h"
#include "realm/sig/ush_comm_realm_sigreg.h"
#include "realm/sig/ush_comm_realm_sigset.h"
#include "realm/sig/ush_comm_realm_sigtease.h"

typedef struct ush_realm_s {
    mqd_t      mq;
    ush_char_t fullname[USH_COMM_CONN_FULL_NAME_LEN_MAX];
} * ush_realm_t;

ush_ret_t
ush_realm_alloc(ush_realm_t *pRealm, const ush_char_t *fullname) {
    ush_assert(pRealm);
    *pRealm = NULL;

    ush_realm_t tmp = (ush_realm_t)malloc(sizeof(struct ush_realm_s));
    if (!tmp) {
        ush_log(LOG_LVL_FATAL, "realm alloc failed");
        return USH_RET_OUT_OF_MEM;
    }

    tmp->mq = USH_INVALID_MQD_VALUE;
    strncpy(tmp->fullname, fullname, sizeof(tmp->fullname));

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


    realm->mq = mq_open(realm->fullname, O_WRONLY);
    if (USH_INVALID_MQD_VALUE == realm->mq) { // failed
        ush_log(LOG_LVL_INFO, "realm open done.");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}

ush_ret_t
ush_realm_send(ush_realm_t realm, const ush_comm_realm_msg_d *msg) {
    if (!realm || !msg) {
        ush_log(LOG_LVL_ERROR, "NULL ptr of params");
        return USH_RET_WRONG_PARAM;
    }

    if (USH_INVALID_MQD_VALUE == realm->mq) {
        ush_log(LOG_LVL_ERROR, "invalid mqueue");
        return USH_RET_WRONG_SEQ;
    }

    ush_size_t sz = 0;
    ush_u32_t  prio = 255; // lowest

    // prepare the sz and prio
    if (USH_COMM_REALM_MSG_CATALOG_CMD == msg->catalog) {
        ush_assert(0);
        // sz = ush_comm_realm_cmd_close_sizeof();
        // prio = USH_COMM_REALM_SEND_PRIO_CMD;

    } else if (USH_COMM_REALM_MSG_CATALOG_SIG == msg->catalog) {
        switch (((const ush_comm_realm_sig_d*)msg)->intent) {
        case USH_COMM_REALM_SIG_INTENT_REG:
            sz = ush_comm_realm_sigreg_sizeof();
            prio = USH_COMM_REALM_SEND_PRIO_SIGREG;
            break;

        case USH_COMM_REALM_SIG_INTENT_SET:
            sz = ush_comm_realm_sigset_sizeof();
            prio = USH_COMM_REALM_SEND_PRIO_SIGSET;
            break;

        case USH_COMM_REALM_SIG_INTENT_TEASE:
            sz = ush_comm_realm_sigtease_sizeof();
            prio = USH_COMM_REALM_SEND_PRIO_SIGTEASE;
            break;

        default:
            ushd_log(LOG_LVL_ERROR, "wrong realm sig msg type");
            break;
        }
    }

    if (sz >= USH_COMM_REALM_Q_MSG_MAX_LEN) {
        ush_log(LOG_LVL_FATAL, "msg tooooooooo long!!!!");
        return USH_RET_FAILED;
    }

    int i = mq_send(realm->mq, (const ush_char_t *)msg, sz, prio);
    if (-1 == i) {
        ush_log(LOG_LVL_FATAL, "send realm msg failed.");
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

    free(*pRealm);
    *pRealm = NULL;

    return USH_RET_OK;
}
