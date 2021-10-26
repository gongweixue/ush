#include "mqueue.h"
#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_log.h"

#include "ush_realm.h"

typedef struct ush_realm {
    mqd_t mq;
    pthread_t  tid;
    ush_char_t name[USH_COMM_CONN_NAME_LEN_MAX];
} * ush_realm_t;

ush_ret_t
ush_realm_alloc(ush_realm_t *pRealm) {
    ush_assert(pRealm);
    *pRealm = NULL;

    ush_realm_t tmp = (ush_realm_t)malloc(sizeof(struct ush_realm));
    if (!tmp) {
        ush_log(LOG_LVL_FATAL, "realm alloc failed");
        return USH_RET_OUT_OF_MEM;
    }
    ush_log(LOG_LVL_DETAIL, "realm heap mem allocate, addr %p", tmp);

    tmp->mq = USH_INVALID_MQD_VALUE;
    *pRealm = tmp;

    *pRealm->tid = USH_INVALID_TID;
    name[0] = '\0'

    return USH_RET_OK;
}
