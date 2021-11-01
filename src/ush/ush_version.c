#include "ush_assert.h"
#include "ush_log.h"
#include "ush_type_pub.h"
#include "ush_version.h"

ush_ret_t
ush_version_get(ush_u32_t *pVerInfo, ush_u16_t msTimeout) {
    ush_assert(pVerInfo);
    if (!pVerInfo) {
        ush_log(LOG_LVL_FATAL, "params error");
        return USH_RET_WRONG_PARAM;
    }
    (void)msTimeout;
    *pVerInfo = 0x00040000; // v0.4
    return USH_RET_OK;
}
