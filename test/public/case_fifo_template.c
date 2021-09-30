#include "common.h"
#include "ush_fifo_template.h"

#include "stdlib.h"
#include "string.h"

static unsigned int cnt = 0;
static const unsigned int max = 4096;
void init() {
}

typedef struct msg {
    size_t     flag;
    ush_s32_t  data;
    ush_u32_t  random;
    ush_s32_t  chk;
} msg_t;

void write_elem(msg_t *dst, const ush_s32_t *src, ush_size_t sz) {
    (void)src;
    ush_assert(dst);
    dst->flag = 1;
    dst->data = rand();
    ush_time_delay_ms(rand() % 4 + 1);
    dst->random = rand();
    dst->chk = (dst->data ^ dst->random);
    cnt++;
}

ush_size_t read_elem(ush_vptr_t dst, const msg_t *src, ush_size_t sz) {
    ush_assert(src->flag == 1);
    (void)dst;
    ush_s32_t data = src->data;
    ush_u32_t r    = src->random;
    ush_s32_t chk  = src->chk;
    ush_time_delay_ms(rand() % 3 + 1);
    ush_assert((data ^ r) == chk);
    cnt++;
    return sz;
}

USH_FIFO_DECL_CODE_GEN(ush_test);

USH_FIFO_IMPL_CODE_GEN(ush_test, 10, msg_t, init, write_elem, read_elem, sizeof(msg_t));




void *thread_push(void *arg) {
    while(cnt < max) {
        ush_test_fifo_push((ush_test_fifo_t)arg, arg, 4);
    }
    return 0;
}

void *thread_pop(void *arg) {
    while(cnt < max) {
        ush_test_fifo_pop((ush_test_fifo_t)arg, arg, sizeof(msg_t));
    }
    return 0;
}

void test_fifo_template() {
    ush_test_fifo_t fifo = ush_test_fifo_create();
    ush_assert(NULL != fifo);

    pthread_t tpush;
    pthread_create(&tpush, NULL, thread_push, fifo);

    pthread_t tpop;
    pthread_create(&tpop, NULL, thread_pop, fifo);

    pthread_join(tpush, NULL);
    pthread_join(tpop, NULL);

}
