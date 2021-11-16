#include "pthread.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_comm_tch.h"
#include "ush_comm_tch_hello.h"
#include "ush_log.h"
#include "ush_type_pub.h"
#include "ush_time.h"

#include "ushd_sched_fifo.h"

////////////////////////////////////////////////////////////////////////////////
// @@@IMPORTANT !!!
// the impelementation should be considered on the performance perspective.
// maybe it need some optimization in the future due to the lock operation.
// or change to C++ with atomic operation and STL, or asm or intrisic-func
////////////////////////////////////////////////////////////////////////////////

#define SCHED_FIFO_ELEM_MAX_NUM (100)

typedef struct elem {
    ush_char_t data[USHD_SCHED_FIFO_ELEM_DATA_LEN];
    ush_size_t sz;
} elem;

static void
fifo_init(ushd_sched_fifo_t fifo) {
    ush_assert(fifo);
    (void)fifo;
}

static void
write_elem(elem *dst, const void *src, ush_size_t sz) {
    ush_assert(sz <= USHD_SCHED_FIFO_ELEM_DATA_LEN && dst && src);
    memcpy(dst->data, src, sz);
    dst->sz = sz;
}

static ush_size_t
read_elem(void *dst, const elem *src, ush_size_t sz) {
    ush_assert(dst && src && sz >= src->sz);
    if (sz < src->sz) {
        return 0;
    }
    memcpy(dst, src->data, src->sz);
    return src->sz;
}

USH_FIFO_IMPL_CODE_GEN(ushd_sched,
                       SCHED_FIFO_ELEM_MAX_NUM,
                       elem,
                       fifo_init,
                       write_elem,
                       read_elem,
                       USHD_SCHED_FIFO_ELEM_DATA_LEN
);




static ushd_sched_fifo_t s_sched_fifo = NULL;
static pthread_mutex_t s_mutex_singleton = PTHREAD_MUTEX_INITIALIZER;

ushd_sched_fifo_t
ushd_sched_fifo_singleton(void) {
    if (!s_sched_fifo) { // null test without lock
        pthread_mutex_lock(&s_mutex_singleton);
        if (!s_sched_fifo) {
            ushd_log(LOG_LVL_INFO, "sched fifo create first time");
            s_sched_fifo = ushd_sched_fifo_create();
        }
        pthread_mutex_unlock(&s_mutex_singleton);
    }
    return s_sched_fifo;
}
