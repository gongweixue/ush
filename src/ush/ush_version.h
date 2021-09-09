#ifndef USH_VERSION_H
#define USH_VERSION_H

#include "ush_type_pub.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Versioning for compatible
 * Use the bit-mask for selecting what you want
 * version format: 0xXXYYZZZZ
 * note: X for major, Y for minor, Z for compatible
 * NO compatibility backward if data in low 16 bits is 0
*/
#define USH_MASK_VERSION_MAJOR        ( 0x00000011 << 24 )
#define USH_MASK_VERSION_MINOR        ( 0x00000011 << 16 )
#define USH_MASK_VERSION_COMPATIBLE   ( 0x00001111 )

/*
 * Get version infomation for confirm the compatiblity with the early version.
 * sync call: y
 * thread safe: y
 * ret: OK FAILED TIMEOUT WRONG_PARAM
 * restriction: pVerInfo should not NULL
 * note: implement with high performance posix or any high performance way
 * detail:
 *     wait until timeout value reached, 0 means forever.
 *     use USH_MASK_VERSION_**** to select the data you want
*/
ush_ret_t ush_version_get(ush_u32_t *pVerInfo, ush_u16_t msTimeout);


#ifdef __cplusplus
}
#endif

#endif // USH_VERSION_H
