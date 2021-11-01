#ifndef USH_FIFO_TEMPLATE_H
#define USH_FIFO_TEMPLATE_H

#include "pthread.h"
#include "stdlib.h"

#include "ush_assert.h"
#include "ush_log.h"
#include "ush_type_pub.h"



#ifndef USH_FIFO_DECL_CODE_GEN

// for the fifo header file
#define USH_FIFO_DECL_CODE_GEN(NAME)                                            \
typedef struct NAME##_fifo_s * NAME##_fifo_t;                                   \
                                                                                \
NAME##_fifo_t NAME##_fifo_create(void);                                         \
ush_ret_t NAME##_fifo_destroy(NAME##_fifo_t *pFifo);                            \
ush_ret_t NAME##_fifo_push(NAME##_fifo_t fifo, const void *buf, ush_size_t sz); \
                                                                                \
ush_size_t NAME##_fifo_pop(NAME##_fifo_t fifo, void *buf, ush_size_t sz);


#endif // USH_FIFO_DECL_CODE_GEN




#ifndef USH_FIFO_IMPL_CODE_GEN

// for the fifo src file
#define USH_FIFO_IMPL_CODE_GEN(NAME,                                            \
                               ELEM_NUM,                                        \
                               ELEM_TYPE,                                       \
                               INIT_ROUTINE,                                    \
                               WRITE_ELEM,                                      \
                               READ_ELEM,                                       \
                               MSG_MAX_LEN)                                     \
                                                                                \
typedef struct NAME##_fifo_s {                                                  \
    pthread_mutex_t mutex;                                                      \
    pthread_cond_t  cond_consumer;                                              \
    pthread_cond_t  cond_producer;                                              \
    ELEM_TYPE       buffer[ELEM_NUM + 1];                                       \
    ush_s32_t       head;                                                       \
    ush_s32_t       tail;                                                       \
} * NAME##_fifo_t;                                                              \
                                                                                \
static const ush_s32_t fifo_len = (ELEM_NUM + 1);                               \
                                                                                \
static ush_ret_t NAME##_fifo_cs_entry(NAME##_fifo_t fifo);                      \
static ush_ret_t NAME##_fifo_cs_exit(NAME##_fifo_t fifo);                       \
static ush_ret_t NAME##_fifo_producers_wait(NAME##_fifo_t fifo);                \
static ush_ret_t NAME##_fifo_consumers_wait(NAME##_fifo_t fifo);                \
static ush_ret_t NAME##_fifo_notify_producers(NAME##_fifo_t fifo);              \
static ush_ret_t NAME##_fifo_notify_consumers(NAME##_fifo_t fifo);              \
static ush_bool_t NAME##_fifo_is_empty(const NAME##_fifo_t fifo);               \
static ush_bool_t NAME##_fifo_is_full(const NAME##_fifo_t fifo);                \
static ush_size_t NAME##_fifo_curr_num(const NAME##_fifo_t fifo);               \
                                                                                \
                                                                                \
NAME##_fifo_t                                                                   \
NAME##_fifo_create(void) {                                                      \
    NAME##_fifo_t fifo = (NAME##_fifo_t)malloc(sizeof(struct NAME##_fifo_s));   \
                                                                                \
    if (!fifo) {                                                                \
        ushd_log(LOG_LVL_ERROR, "fifo of "#NAME" creation failed");             \
        goto RET;                                                               \
    }                                                                           \
                                                                                \
    if (0 != pthread_mutex_init(&fifo->mutex, NULL)) {                          \
        ushd_log(LOG_LVL_ERROR, "fifo of "#NAME" mutex init failed");           \
        goto BAILED_FIFO;                                                       \
    }                                                                           \
                                                                                \
    if (0 != pthread_cond_init(&fifo->cond_consumer, NULL)) {                   \
        ushd_log(LOG_LVL_ERROR, "fifo of "#NAME" consumer cond init failed");   \
        goto BAILED_MUTEX;                                                      \
    }                                                                           \
    if (0 != pthread_cond_init(&fifo->cond_producer, NULL)) {                   \
        ushd_log(LOG_LVL_ERROR, "fifo of "#NAME" producer cond init failed");   \
        goto BAILED_COND_CONSUMER;                                              \
    }                                                                           \
                                                                                \
    fifo->head = 0;                                                             \
    fifo->tail = 0;                                                             \
    INIT_ROUTINE(fifo);                                                         \
    goto RET;                                                                   \
                                                                                \
BAILED_COND_CONSUMER:                                                           \
    pthread_cond_destroy(&fifo->cond_consumer);                                 \
BAILED_MUTEX:                                                                   \
    pthread_mutex_destroy(&fifo->mutex);                                        \
BAILED_FIFO:                                                                    \
    ushd_log(LOG_LVL_DETAIL, "free fifo %p", fifo);                             \
    free(fifo);                                                                 \
    fifo = NULL;                                                                \
RET:                                                                            \
    return fifo;                                                                \
}                                                                               \
                                                                                \
ush_ret_t                                                                       \
NAME##_fifo_destroy(NAME##_fifo_t *pFifo) {                                     \
    if (pFifo && *pFifo) {                                                      \
        ushd_log(LOG_LVL_ERROR, "destroy null ptr fifo of %s", #NAME);          \
        return USH_RET_WRONG_PARAM;                                             \
    }                                                                           \
    NAME##_fifo_cs_entry(*pFifo);                                               \
    pthread_cond_destroy(&((*pFifo)->cond_producer));                           \
    pthread_cond_destroy(&((*pFifo)->cond_consumer));                           \
    NAME##_fifo_cs_exit(*pFifo);                                                \
    pthread_mutex_destroy(&((*pFifo)->mutex));                                  \
    return USH_RET_OK;                                                          \
}                                                                               \
                                                                                \
ush_ret_t                                                                       \
NAME##_fifo_push(NAME##_fifo_t fifo, const void *buf, ush_size_t sz) {          \
    ush_assert(fifo && buf && sz <= MSG_MAX_LEN);                               \
                                                                                \
    NAME##_fifo_cs_entry(fifo);                                                 \
                                                                                \
    while (NAME##_fifo_is_full(fifo)) {                                         \
        ushd_log(LOG_LVL_INFO, "waiting "#NAME" fifo space...");                \
        NAME##_fifo_producers_wait(fifo);                                       \
    }                                                                           \
                                                                                \
    WRITE_ELEM(fifo->buffer + fifo->tail, buf, sz);                             \
    fifo->tail = (fifo->tail + 1) % fifo_len;                                   \
                                                                                \
    if (NAME##_fifo_curr_num(fifo) == 1) {                                      \
        NAME##_fifo_notify_consumers(fifo);                                     \
    }                                                                           \
    NAME##_fifo_cs_exit(fifo);                                                  \
                                                                                \
    ushd_log(LOG_LVL_INFO, "data pushed to "#NAME" fifo");                      \
                                                                                \
    return USH_RET_OK;                                                          \
}                                                                               \
                                                                                \
ush_size_t                                                                      \
NAME##_fifo_pop(NAME##_fifo_t fifo, void *buf, ush_size_t sz) {                 \
    ush_assert(fifo && buf && sz >= MSG_MAX_LEN);                               \
                                                                                \
    NAME##_fifo_cs_entry(fifo);                                                 \
                                                                                \
    while (NAME##_fifo_is_empty(fifo)) {                                        \
        ushd_log(LOG_LVL_INFO, "waiting "#NAME" fifo item arrived...");         \
        NAME##_fifo_consumers_wait(fifo);                                       \
    }                                                                           \
                                                                                \
    ush_size_t ret = READ_ELEM(buf, fifo->buffer + fifo->head, sz);             \
    if (USH_FALSE == ret) {                                                     \
        ushd_log(LOG_LVL_ERROR, "pop elem to %p failed.", buf);                 \
        goto BAILED;                                                            \
    }                                                                           \
    fifo->head = (fifo->head + 1) % fifo_len;                                   \
                                                                                \
    if (NAME##_fifo_curr_num(fifo) == ELEM_NUM-1) {                             \
        NAME##_fifo_notify_producers(fifo);                                     \
    }                                                                           \
    ushd_log(LOG_LVL_INFO, "data pop from "#NAME" fifo");                       \
                                                                                \
BAILED:                                                                         \
    NAME##_fifo_cs_exit(fifo);                                                  \
    return ret;                                                                 \
}                                                                               \
                                                                                \
static ush_ret_t                                                                \
NAME##_fifo_cs_entry(NAME##_fifo_t fifo) {                                      \
    ush_assert(fifo);                                                           \
    if (!fifo) {                                                                \
        return USH_RET_WRONG_PARAM;                                             \
    }                                                                           \
    if (0 != pthread_mutex_lock(&fifo->mutex)) {                                \
        ushd_log(LOG_LVL_ERROR, "entry "#NAME" fifo %p failed", fifo);          \
        return USH_RET_FAILED;                                                  \
    }                                                                           \
    ushd_log(LOG_LVL_DETAIL, "entry "#NAME" fifo %p", fifo);                    \
    return USH_RET_OK;                                                          \
}                                                                               \
                                                                                \
static ush_ret_t                                                                \
NAME##_fifo_cs_exit(NAME##_fifo_t fifo) {                                       \
    ush_assert(fifo);                                                           \
    if (!fifo) {                                                                \
        return USH_RET_WRONG_PARAM;                                             \
    }                                                                           \
    if (0 != pthread_mutex_unlock(&fifo->mutex)) {                              \
        ushd_log(LOG_LVL_ERROR, "exit "#NAME" fifo %p failed", fifo);           \
        return USH_RET_FAILED;                                                  \
    }                                                                           \
    ushd_log(LOG_LVL_DETAIL, "exit "#NAME" fifo %p", fifo);                     \
    return USH_RET_OK;                                                          \
}                                                                               \
                                                                                \
static ush_ret_t                                                                \
NAME##_fifo_producers_wait(NAME##_fifo_t fifo) {                                \
    ush_assert(fifo);                                                           \
    if (!fifo) {                                                                \
        return USH_RET_WRONG_PARAM;                                             \
    }                                                                           \
    ushd_log(LOG_LVL_DETAIL, "producer waiting...");                            \
    if (0 != pthread_cond_wait(&fifo->cond_producer, &fifo->mutex)) {           \
        return USH_RET_FAILED;                                                  \
    }                                                                           \
    return USH_RET_OK;                                                          \
}                                                                               \
                                                                                \
static ush_ret_t                                                                \
NAME##_fifo_consumers_wait(NAME##_fifo_t fifo) {                                \
    ush_assert(fifo);                                                           \
    if (!fifo) {                                                                \
        return USH_RET_WRONG_PARAM;                                             \
    }                                                                           \
    ushd_log(LOG_LVL_DETAIL, "consumer waiting...");                            \
    if (0 != pthread_cond_wait(&fifo->cond_consumer, &fifo->mutex)) {           \
        return USH_RET_FAILED;                                                  \
    }                                                                           \
    return USH_RET_OK;                                                          \
}                                                                               \
                                                                                \
static ush_ret_t                                                                \
NAME##_fifo_notify_producers(NAME##_fifo_t fifo) {                              \
    ush_assert(fifo);                                                           \
    if (!fifo) {                                                                \
        return USH_RET_WRONG_PARAM;                                             \
    }                                                                           \
    ushd_log(LOG_LVL_DETAIL, "producer trigger...");                            \
    if (0 != pthread_cond_signal(&fifo->cond_producer)) {                       \
        return USH_RET_FAILED;                                                  \
    }                                                                           \
    return USH_RET_OK;                                                          \
}                                                                               \
                                                                                \
static ush_ret_t                                                                \
NAME##_fifo_notify_consumers(NAME##_fifo_t fifo) {                              \
    ush_assert(fifo);                                                           \
    if (!fifo) {                                                                \
        return USH_RET_WRONG_PARAM;                                             \
    }                                                                           \
    ushd_log(LOG_LVL_DETAIL, "comsumer trigger...");                            \
    if (0 != pthread_cond_signal(&fifo->cond_consumer)) {                       \
        return USH_RET_FAILED;                                                  \
    }                                                                           \
    return USH_RET_OK;                                                          \
}                                                                               \
                                                                                \
static ush_bool_t                                                               \
NAME##_fifo_is_empty(const NAME##_fifo_t fifo) {                                \
    ush_assert(fifo);                                                           \
    if (!fifo) {                                                                \
        return USH_FALSE;                                                       \
    }                                                                           \
    ush_bool_t ret = (fifo->head == fifo->tail) ? USH_TRUE : USH_FALSE;         \
    return ret;                                                                 \
}                                                                               \
                                                                                \
static ush_bool_t                                                               \
NAME##_fifo_is_full(const NAME##_fifo_t fifo) {                                 \
    ush_assert(fifo);                                                           \
    if (!fifo) {                                                                \
        return USH_FALSE;                                                       \
    }                                                                           \
    ush_bool_t ret = ((fifo->tail + 1) % fifo_len) == (fifo->head);             \
    return ret;                                                                 \
}                                                                               \
                                                                                \
static ush_size_t                                                               \
NAME##_fifo_curr_num(const NAME##_fifo_t fifo) {                                \
    ush_assert(fifo);                                                           \
    if (!fifo) {                                                                \
        return USH_RET_WRONG_PARAM;                                             \
    }                                                                           \
    return (ush_size_t)((fifo->tail + fifo_len - fifo->head) % fifo_len);       \
}                                                                               \


#endif // USH_FIFO_IMPL_CODE_GEN



#endif // USH_FIFO_TEMPLATE_H
