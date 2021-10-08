#include "stdlib.h"
#include "string.h"

#include "ush_comm_def.h"
#include "ush_type_pub.h"

#include "ushd_reg_list.h"

//   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
//             ^   ^   ^   ^   ^   ^
//             |   |   |   |   |   |____ subscribe flg
//             |   |   |   |   |________ verbose flg
//             |   |   |___|
//             |   |     |______________ interval timeslice base
//             |___|                     0: none, 1: 10ms, 2: 100ms, 3: 1sec
//               |
//               |______________________ interval timeslice factor
//                                       0: none, 1: x1,  2: x2,  3: x5

typedef unsigned char REG_LIST_ELEM_TYPE;
typedef enum {
    INTERVAL_BASE_NONE,
    INTERVAL_BASE_10ms,
    INTERVAL_BASE_100ms,
    INTERVAL_BASE_1s,
} INTERVAL_BASE;
typedef enum {
    INTERVAL_FCTR_NONE,
    INTERVAL_FCTR_x1,
    INTERVAL_FCTR_x2,
    INTERVAL_FCTR_x5,
} INTERVAL_FCTR;

typedef struct ushd_reg_list {
    REG_LIST_ELEM_TYPE bitmap[USH_COMM_SIG_IDX_MAX];
} * ushd_reg_list_t;

#define SHFT_SUBSCRIBE            (0)
#define MASK_SUBSCRIBE            (0x01) // 00000001b

#define SHFT_VERBOSE              (1)
#define MASK_VERBOSE              (0x02) // 00000010b

#define SHFT_INTERVALBASE         (2)
#define MASK_INTERVALBASE         (0x0C) // 00001100b

#define SHFT_INTERVALFCTR         (4)
#define MASK_INTERVALFCTR         (0x30) // 00110000b

#define DEFINE_RW_FUNC_OF(NAME)                                             \
static inline void SET_##NAME##_OF(REG_LIST_ELEM_TYPE *ptr, ush_u8_t val) { \
    *ptr = ((*ptr) & (~MASK_##NAME)) | (val << SHFT_##NAME);                \
}                                                                           \
static inline ush_u8_t GET_##NAME##_OF(REG_LIST_ELEM_TYPE *ptr) {           \
    return (*ptr & MASK_##NAME) >> SHFT_##NAME;                             \
}

DEFINE_RW_FUNC_OF(SUBSCRIBE);
DEFINE_RW_FUNC_OF(VERBOSE);
DEFINE_RW_FUNC_OF(INTERVALBASE);
DEFINE_RW_FUNC_OF(INTERVALFCTR);


ushd_reg_list_t
ushd_reg_list_create() {
    ushd_reg_list_t ret = (ushd_reg_list_t)malloc(sizeof(struct ushd_reg_list));
    if (!ret) {
        return NULL;
    }
    memset(ret->bitmap, 0, sizeof(ret->bitmap));

    return ret;
}
