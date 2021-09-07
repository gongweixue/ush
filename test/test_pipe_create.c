#include "ush_pipe.h"

#include "stdio.h"

ush_ret_t ush_pipe_create(
    const ush_char_t *pName,         // pipe name
    ush_pp_mode_t     mode,          // std / bunch
    ush_u32_t         flag,          // bit-mask for indicating tracing/wdg/logging/statistics and so on.
    ush_u16_t         msTimeout,     // the unit is 1s at least, and 0 is forever.
    ush_vptr_t       *pParams,       // 2-level ptrs for all params, conf-file, etc.
    ush_size_t        paramCnt,
    ush_pp_hdl_t     *pHdl);

int main () {
    ush_pp_hdl_t hdl;
    ush_ret_t ret = ush_pipe_create("ping", 0, 0, 0, NULL, 0, &hdl);
    printf("create return %d\n", ret);

    while(1) ;
    return 0;
}
