#ifndef USH_PIPE_PUB_H
#define USH_PIPE_PUB_H

/*******************************************************************************
 * Header file of the lib for the feature of signal router -- Uni-Signal-Hub
 *
 * **Sync call: wait until the real result returns.
 * **Thread safe: it can not be ensure that the call return the correct
 * **result if you use the same context in other thread at the same time.
 * **Keep smart when use CB function if you use C++ as the client language.
*******************************************************************************/
#include "ush_type_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Attr for pipe */
typedef enum USH_PIPE_ATTR {
    USH_PP_ATTR_NAME    = 0,
    USH_PP_ATTR_STATE   = 1,
    USH_PP_ATTR_MODE    = 2,
    USH_PP_ATTR_FLAG    = 3, // trace, wdg, log, statistics, etc.
} USH_PIPE_ATTR;

#define USH_PP_ATTR_FLG_MASK_WDG  (0x0000000F) // 4bits for 0-15s, 0 for no wdg
#define USH_PP_ATTR_FLG_SHFT_WDG  (0)


typedef enum USH_PIPE_MODE {
    USH_PP_MODE_STD   = 0,
    USH_PP_MODE_BUNCH = 1, // **DO NOT** USE CURRENTLY!!!
    USH_PP_MODE_MAX_GUARD,
} USH_PIPE_MODE;

/*
 * Create a pipe with hub service
 * ret: OK FAILED WRONG_PARAM WRONG_SEQ NO_DATA TIMEOUT
 * restriction: pName/pHdl should not NULL nor too long (less than 20 is fair)
 * detail:
 *     wait until timeout value reached, 0 means forever. Handle could be passed
 *     to other threads for registering or querying operations, starting, stopping,
 *     but not destructing. The hub deals the request with the arrived order.
 *     Flag is bitmask data, not used currently, default is 0.
 *     Mode is the enum for the interactive with the pipe, USH_PP_MODE_STD is the default
*/
ush_ret_t ush_pipe_create(
    const ush_char_t *pName,         // pipe name
    USH_PIPE_MODE     mode,          // std / bunch
    ush_u32_t         flag,          // bit-mask for indicating tracing/wdg/logging/statistics and so on.
    ush_u16_t         timeout,       // the unit is 1s at least, and 0 is forever.
    ush_pvoid_t      *pParams,       // 2-level ptrs for all params, conf-file, etc.
    ush_size_t        paramCnt,
    ush_pipe_t       *pPipe);

/*
 * Destory a pipe connecting with the hub
 * sync call: Y
 * ret: OK FAILED WRONG_PARAM WRONG_SEQ
 * restriction: handle should be not NULL
 * detail: handle be delete, invalid pipe leads UB
*/
ush_ret_t ush_pipe_delete(ush_pipe_t pipe);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // USH_PIPE_PUB_H
