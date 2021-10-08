#include "pthread.h"
#include "stdlib.h"
#include "string.h"

#include "ush_assert.h"
#include "ush_log.h"
#include "ush_type_pub.h"

#include "ushd_dist_fifo.h"


#define DIST_FIFO_ELEM_MAX_NUM (128)

typedef struct elem {
    ush_char_t data[USHD_DIST_FIFO_ELEM_DATA_LEN];
    ush_size_t sz;
} elem;

static void
fifo_init(ushd_dist_fifo_t fifo) {
    ush_assert(fifo);
}

static void
write_elem(elem *dst, const void *src, ush_size_t sz) {
    ush_assert(sz <= USHD_DIST_FIFO_ELEM_DATA_LEN && dst && src);
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

USH_FIFO_IMPL_CODE_GEN(ushd_dist,
                       DIST_FIFO_ELEM_MAX_NUM,
                       elem,
                       fifo_init,
                       write_elem,
                       read_elem,
                       USHD_DIST_FIFO_ELEM_DATA_LEN
);
