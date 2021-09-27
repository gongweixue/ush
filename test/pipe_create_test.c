#include "ush_pipe_pub.h"

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

void itoa(int a, char *str)
{
    char *beg = str;
    int sign;
    if ((sign = a) < 0) a = -a;

    do {
        *str++ = '0' + a % 10;
    } while((a /= 10) > 0);

    if (sign < 0) *str++ = '-';

    *str = '\0';

    char *end = str - 1;
    while (beg < end){
        char tmp = *beg;
        *beg++   = *end;
        *end--   = tmp;
    }
}

int main () {
    ush_pp_hdl_t hdl;
    ush_ret_t ret = USH_RET_OK;
    ush_char_t name[64];
    itoa(time(NULL), name);
    ret = ush_pipe_create(name, 0, 0, 2, NULL, 0, &hdl);
    printf("create return %d\n", ret);

    // ret = ush_pipe_create("WWW", 0, 0, 0, NULL, 0, &hdl);
    // printf("create return %d and back to main\n", ret);

    while(1) {
    }
    return 0;
}
