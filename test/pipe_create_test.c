#include "ush_pipe_pub.h"
#include "ush_string.h"

#include "stdio.h"
#include "time.h"
#include "unistd.h"

ush_ret_t ush_pipe_create(
    const ush_char_t *pName,         // pipe name
    USH_PIPE_MODE     mode,          // std / bunch
    ush_u32_t         flag,          // bit-mask for indicating tracing/wdg/logging/statistics and so on.
    ush_u16_t         msTimeout,     // the unit is 1s at least, and 0 is forever.
    ush_vptr_t       *pParams,       // 2-level ptrs for all params, conf-file, etc.
    ush_size_t        paramCnt,
    ush_pp_hdl_t     *pHdl);

int main () {
    ush_pp_hdl_t hdl;
    ush_ret_t ret = USH_RET_OK;
    ush_char_t name[64];
    ush_itoa(name, time(NULL));
    // ret = ush_pipe_create(name, 0, 0, 2, NULL, 0, &hdl);
    // printf("create return %d\n", ret);

    ret = ush_pipe_create("AAA", 0, 0, 0, NULL, 0, &hdl);
    printf("create return %d\n", ret);

    while(1) {
    }
    return 0;
}
