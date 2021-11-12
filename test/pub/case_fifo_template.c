
#include "stdlib.h"
#include "string.h"

#include "test-common.h"
#include "ush_fifo_template.h"


static unsigned int cnt = 0;
static const unsigned int max = 1024;
static void init(ush_pvoid_t fifo) {
    (void)fifo;
}

typedef struct {
    size_t     flag;
    ush_s32_t  data;
    ush_s32_t  random;
    ush_s32_t  chk;
} msg_s;

static void write_elem(msg_s *dst, const ush_s32_t *src, ush_size_t sz) {
    (void)src;
    (void)sz;
    ush_assert(dst);
    dst->flag = 1;
    dst->data = rand();
    ush_time_delay_ms((unsigned int)rand() % 4 + 1);
    dst->random = rand();
    dst->chk = (dst->data ^ dst->random);
    cnt++;
}

static ush_size_t read_elem(ush_pvoid_t dst, const msg_s *src, ush_size_t sz) {
    ush_assert(src->flag == 1);
    (void)dst;
    (void)sz;
    ush_s32_t data = src->data;
    ush_s32_t r    = src->random;
    ush_s32_t chk  = src->chk;
    ush_time_delay_ms((unsigned int)rand() % 3 + 1);
    ush_assert((data ^ r) == chk);
    cnt++;
    return sz;
}

USH_FIFO_DECL_CODE_GEN(ush_test);

USH_FIFO_IMPL_CODE_GEN(ush_test, 10, msg_s, init,
                       write_elem, read_elem,
                       sizeof(msg_s));




static void *thread_push(void *arg) {
    while(cnt < max) {
        ush_test_fifo_push((ush_test_fifo_t)arg, arg, 4);
    }
    return 0;
}

static void *thread_pop(void *arg) {
    while(cnt < max) {
        ush_test_fifo_pop((ush_test_fifo_t)arg, arg, sizeof(msg_s));
    }
    return 0;
}

static void test_fifo_template(void) {
    ush_test_fifo_t fifo = ush_test_fifo_create();
    ush_assert(NULL != fifo);

    pthread_t tpush;
    pthread_create(&tpush, NULL, thread_push, fifo);

    pthread_t tpop;
    pthread_create(&tpop, NULL, thread_pop, fifo);

    pthread_join(tpush, NULL);
    pthread_join(tpop, NULL);

}
