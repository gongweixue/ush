#include "ush_type.h"
#include "ush_version.h"

ush_ret_t ush_version_get(ush_u32_t *pVerInfo, ush_u16_t msTimeout) {
    if (!pVerInfo) {
        return USH_RET_WRONG_PARAM;
    }
    (void)msTimeout;
    *pVerInfo = 0x00010000; // v0.1
    return USH_RET_OK;
}
