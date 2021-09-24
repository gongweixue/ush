#include "pthread.h"
#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ushd_publish_fifo.h"

#define FIFO_ITEM_NUM    (64)
#define FIFO_SIZE        (FIFO_ITEM_NUM + 1) // extra 1 for guardian of full

typedef struct fifo_msg {
    ush_size_t sz;
    ush_char_t data[USHD_PUBLISH_FIFO_MSG_MAX_SIZE];
} fifo_msg;

typedef struct publish_fifo {
    pthread_mutex_t mutex;
    pthread_cond_t  cond_consumer;
    pthread_cond_t  cond_producer;
    fifo_msg        buffer[FIFO_SIZE];
    ush_s32_t       head;
    ush_s32_t       tail;
} * ushd_publish_fifo_t;


static ush_ret_t fifo_cs_entry(ushd_publish_fifo_t fifo);
static ush_ret_t fifo_cs_exit(ushd_publish_fifo_t fifo);

static ush_ret_t fifo_producers_wait(ushd_publish_fifo_t fifo);
static ush_ret_t fifo_consumers_wait(ushd_publish_fifo_t fifo);
static ush_ret_t fifo_notify_producers(ushd_publish_fifo_t fifo);
static ush_ret_t fifo_notify_consumers(ushd_publish_fifo_t fifo);

static ush_bool_t fifo_is_empty(const ushd_publish_fifo_t fifo);
static ush_bool_t fifo_is_full(const ushd_publish_fifo_t fifo);
static ush_size_t fifo_curr_num(const ushd_publish_fifo_t fifo);


ushd_publish_fifo_t ushd_publish_fifo_create() {
    ushd_publish_fifo_t fifo =
        (ushd_publish_fifo_t)malloc(sizeof(struct publish_fifo));
    if (!fifo) {
        goto RET;
    }

    if (0 != pthread_mutex_init(&fifo->mutex, NULL)) {
        goto BAILED_FIFO;
    }

    if (0 != pthread_cond_init(&fifo->cond_consumer, NULL)) {
        goto BAILED_MUTEX;
    }
    if (0 != pthread_cond_init(&fifo->cond_producer, NULL)) {
        goto BAILED_COND_CONSUMER;
    }

    // set fifo empty
    fifo->head = 0;
    fifo->tail = 0;
    goto RET;

BAILED_COND_CONSUMER:
    pthread_cond_destroy(&fifo->cond_consumer);

BAILED_MUTEX:
    pthread_mutex_destroy(&fifo->mutex);

BAILED_FIFO:
    free(fifo);
    fifo = NULL;

RET:
    return fifo;

}


/*
 * TODO: performance need to be optimized due to the mutex&cond on the fifo
*/
ush_ret_t
ushd_publish_fifo_push(ushd_publish_fifo_t fifo,
                       const ush_vptr_t buf,
                       ush_size_t sz) {
    ush_assert(fifo && buf && sz <= USHD_PUBLISH_FIFO_MSG_MAX_SIZE);

    fifo_cs_entry(fifo);

    while (fifo_is_full(fifo)) { // for multi producer racing
        fifo_producers_wait(fifo);
    }

    int idx = fifo->tail;
    memcpy(fifo->buffer[idx].data, buf, sz);
    fifo->buffer[idx].sz = sz;
    fifo->tail = (fifo->tail + 1) % FIFO_SIZE;

    if (fifo_curr_num(fifo) == 1) { // from 'empty' to 'not empty' state
        fifo_notify_consumers(fifo); // before cs exit to avoid prio-reverse
    }
    fifo_cs_exit(fifo);

    return USH_RET_OK;
}

ush_size_t
ushd_publish_fifo_pop(ushd_publish_fifo_t fifo, ush_vptr_t buf, ush_size_t sz) {
    ush_assert(fifo && buf && sz >= USHD_PUBLISH_FIFO_MSG_MAX_SIZE);

    fifo_cs_entry(fifo);

    while (fifo_is_empty(fifo)) {
        fifo_consumers_wait(fifo);
    }

    int idx = fifo->head;
    ush_size_t ret = 0;
    if (fifo->buffer[idx].sz > sz) {
        ushd_log(LOG_LVL_ERROR, "buf %p size too small.", buf);
        ret = 0;
        goto BAILED;
    }
    memcpy(buf, fifo->buffer[idx].data, fifo->buffer[idx].sz);
    fifo->head = fifo->head + 1 % FIFO_SIZE;

    if (fifo_curr_num(fifo) == FIFO_ITEM_NUM-1) { // from 'full' to 'not full'
        fifo_notify_producers(fifo);
    }

BAILED:
    fifo_cs_exit(fifo);

    return ret;
}

static ush_ret_t
fifo_cs_entry(ushd_publish_fifo_t fifo) {
    ush_assert(fifo);
    if (!fifo) {
        return USH_RET_WRONG_PARAM;
    }
    if (0 != pthread_mutex_lock(&fifo->mutex)) {
        ushd_log(LOG_LVL_ERROR, "entry fifo %p failed", fifo);
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_DETAIL, "entry fifo %p", fifo);
    return USH_RET_OK;
}
static ush_ret_t
fifo_cs_exit(ushd_publish_fifo_t fifo) {
    ush_assert(fifo);
    if (!fifo) {
        return USH_RET_WRONG_PARAM;
    }
    if (0 != pthread_mutex_unlock(&fifo->mutex)) {
        ushd_log(LOG_LVL_ERROR, "exit fifo %p failed", fifo);
        return USH_RET_FAILED;
    }
    ushd_log(LOG_LVL_DETAIL, "exit fifo %p", fifo);
    return USH_RET_OK;
}

static ush_ret_t fifo_producers_wait(ushd_publish_fifo_t fifo) {
    ush_assert(fifo);
    if (!fifo) {
        return USH_RET_WRONG_PARAM;
    }
    if (0 != pthread_cond_wait(&fifo->cond_producer, &fifo->mutex)) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}
static ush_ret_t fifo_consumers_wait(ushd_publish_fifo_t fifo) {
    ush_assert(fifo);
    if (!fifo) {
        return USH_RET_WRONG_PARAM;
    }
    if (0 != pthread_cond_wait(&fifo->cond_consumer, &fifo->mutex)) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}
static ush_ret_t fifo_notify_producers(ushd_publish_fifo_t fifo) {
    ush_assert(fifo);
    if (!fifo) {
        return USH_RET_WRONG_PARAM;
    }
    if (0 != pthread_cond_signal(&fifo->cond_producer)) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}
static ush_ret_t fifo_notify_consumers(ushd_publish_fifo_t fifo) {
    ush_assert(fifo);
    if (!fifo) {
        return USH_RET_WRONG_PARAM;
    }
    if (0 != pthread_cond_signal(&fifo->cond_consumer)) {
        return USH_RET_FAILED;
    }
    return USH_RET_OK;
}

static ush_bool_t
fifo_is_empty(const ushd_publish_fifo_t fifo) {
    ush_assert(fifo);
    if (!fifo) {
        return USH_RET_WRONG_PARAM;
    }
    return (fifo->head == fifo->tail);
}
static ush_bool_t
fifo_is_full(const ushd_publish_fifo_t fifo) {
    ush_assert(fifo);
    if (!fifo) {
        return USH_RET_WRONG_PARAM;
    }
    return ((fifo->tail + 1) % FIFO_SIZE) == (fifo->head);
}
static ush_size_t
fifo_curr_num(const ushd_publish_fifo_t fifo) {
    ush_assert(fifo);
    if (!fifo) {
        return USH_RET_WRONG_PARAM;
    }
    return (fifo->tail + FIFO_SIZE - fifo->head) % FIFO_SIZE;
}
