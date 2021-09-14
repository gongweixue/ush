
#include "errno.h"
#include "mqueue.h"
#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_comm_listener.h"
#include "ush_listener.h"
#include "ush_log.h"

typedef struct ush_listener {
    mqd_t mq;
} * ush_listener_t;


ush_ret_t
ush_listener_alloc(ush_listener_t *pListener) {
    ush_assert(pListener);

    *pListener = NULL;

    ush_listener_t tmp = (ush_listener_t)malloc(sizeof(struct ush_listener));
    if (!tmp) {
        ush_log(LOG_LVL_ERROR, "listener alloc failed\n");
        return USH_RET_OUT_OF_MEM;
    }

    tmp->mq = -1;
    *pListener = tmp;

    return USH_RET_OK;
}

ush_ret_t
ush_listener_open(ush_listener_t listener, const ush_char_t *path) {
    ush_assert(listener && path);
    if (-1 != listener->mq) { // maybe already opened
        return USH_RET_OK;
    }

    ush_assert(strlen(USH_COMM_LISTENER_PATH_PREFIX) + strlen(path)
               < USH_COMM_LISTENER_NAME_LEN_MAX);
    char name[USH_COMM_LISTENER_NAME_LEN_MAX];
    strcpy(name, USH_COMM_LISTENER_PATH_PREFIX);
    strcat(name, path);

    listener->mq = mq_open(name, O_WRONLY);
    if (-1 == listener->mq) {
        ush_log(LOG_LVL_ERROR, "listener open returns failed\n");
        return USH_RET_FAILED;
    }

    return USH_RET_OK;
}

ush_ret_t
ush_listener_close(ush_listener_t listener) {
    ush_assert(listener);
    if (-1 == listener->mq) {
        return USH_RET_OK;
    }

    if (0 != mq_close(listener->mq)) {
        return USH_RET_FAILED;
    }

    listener->mq = -1;

    return USH_RET_OK;
}

ush_ret_t
ush_listener_destroy_with_closing(ush_listener_t *pListener) {
    assert(pListener);
    if (!(*pListener)) {
        ush_log(LOG_LVL_INFO, "ush_listener_t NULL to be destroy\n");
        return USH_RET_OK;
    }

    // close it anyway, no matter if it has been opened.
    ush_listener_close(*pListener);

    free(*pListener);
    *pListener = NULL;

    return USH_RET_OK;
}
