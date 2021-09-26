#include "ush_fifo_template.h"

#include "stdlib.h"
#include "string.h"

void init() {}

typedef struct msg {
    size_t sz;
    ush_char_t data[16];
} msg_t;

void write_elem(msg_t *dst, const ush_vptr_t buf, ush_size_t sz) {
    memcpy(dst->data, buf, sz);
    dst->sz = sz;
}

ush_size_t read_elem(ush_vptr_t buf, const msg_t *src, ush_size_t sz) {
    if (sz < src->sz) {
        return 0;
    }
    memcpy(buf, src->data, sz);
}

USH_FIFO_DECL_CODE_GEN(ush_public);

USH_FIFO_IMPL_CODE_GEN(ushd_sched, 10, msg_t, init, write_elem, read_elem, 128);

void foo() {return;}
