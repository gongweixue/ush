#ifndef USH_TYPE_H
#define USH_TYPE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char      ush_bool_t;
typedef unsigned char      ush_u8_t;
typedef signed char        ush_s8_t;
typedef unsigned short     ush_u16_t;
typedef signed short       ush_s16_t;
typedef unsigned int       ush_u32_t;
typedef signed int         ush_s32_t;
typedef long               ush_long_t;
typedef unsigned long long ush_u64_t;
typedef signed long long   ush_s64_t;
typedef float              ush_fp32_t;
typedef double             ush_fp64_t;
typedef char               ush_char_t;
typedef size_t             ush_size_t;
typedef long               ush_ssize_t;
typedef void *             ush_vptr_t;

#ifndef NULL
#define NULL ((ush_vptr_t)0)
#endif


#define USH_MASK_RET_CATALOG  ( 0x10 )
typedef enum ush_ret_t {
    USH_RET_OK          = 0x00,
    USH_RET_FAILED      = 0x01,
    USH_RET_WRONG_PARAM = 0x10, // 8 bits, the low 4 bits for details extention.
    USH_RET_WRONG_SEQ   = 0x20, // wrong sequence on the pipe state-changing
    USH_RET_NO_DATA     = 0x30,
    USH_RET_NOT_SUPPORT = 0x40,
    USH_RET_TIMEOUT     = 0x50,
} ush_ret_t;

// handle type for pipe, just a number, no meaning on client.
typedef ush_s32_t          ush_pp_hdl_t;


#ifdef __cplusplus
}
#endif

#endif // USH_TYPE_H
