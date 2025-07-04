#ifndef USH_TYPE_PUB_H
#define USH_TYPE_PUB_H

#include <stddef.h>
#include "ush_def_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    USH_FALSE = 0,
    USH_TRUE = !USH_FALSE
} ush_bool_t;

typedef char               ush_char_t;
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
typedef size_t             ush_size_t;
typedef long               ush_ssize_t;
typedef void *             ush_pvoid_t;


typedef ush_bool_t         BOOL;
typedef ush_char_t         CHAR;
typedef ush_u8_t           U8;
typedef ush_s8_t           S8;
typedef ush_u16_t          U16;
typedef ush_s16_t          S16;
typedef ush_u32_t          U32;
typedef ush_s32_t          S32;
typedef ush_u64_t          U64;
typedef ush_s64_t          S64;
typedef ush_fp32_t         FP32;
typedef ush_fp64_t         FP64;


#define USH_MASK_RET_CATALOG  ( 0x10 )
typedef enum ush_ret {
    USH_RET_OK          = 0x00,
    USH_RET_FAILED      = 0x01,
    USH_RET_WRONG_PARAM = 0x10, // 8 bits, the low 4 bits for details extention.
    USH_RET_WRONG_SEQ   = 0x20, // wrong sequence on the pipe state-changing
    USH_RET_OUT_OF_MEM  = 0x30,
    USH_RET_NOT_SUPPORT = 0x40,
    USH_RET_TIMEOUT     = 0x50,
} ush_ret_t;

// handle type for pipe, just a number, no meaning on client.
typedef ush_u64_t          ush_pipe_t;

typedef ush_s32_t          ush_connidx_t;
typedef ush_s32_t          ush_cert_t;


// union for signal value
typedef union ush_sig_val_t {
    BOOL    dataBOOL;
    CHAR    dataCHAR;
    U8      dataU8;
    S8      dataS8;
    U16     dataU16;
    S16     dataS16;
    U32     dataU32;
    S32     dataS32;
    U64     dataU64;
    S64     dataS64;
    FP32    dataFP32;
    FP64    dataFP64;
    U64     dataBits; // bits to copy and equal-test(syntax suggar)
} ush_sig_val_t;

#ifdef __cplusplus
}
#endif

#endif // USH_TYPE_PUB_H
