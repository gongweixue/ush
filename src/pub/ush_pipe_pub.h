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

#define USH_PP_ATTR_FLG_MASK_WDG  (0x0000000F) // 4bits for 0-15s, 0 for no wdg
#define USH_PP_ATTR_FLG_SHFT_WDG  (0)

/*
 * Create a pipe with hub service
 * ret: OK FAILED WRONG_PARAM WRONG_SEQ NO_DATA TIMEOUT
 * restriction: pName/pHdl should not NULL nor too long (less than 20 is fair)
 * detail:
 *     wait until timeout value reached, 0 means forever. Handle could be passed
 *     to other threads for registering, but not destructing.
 *     The hub deals the request with the arrived order.
 *     Flag is bitmask data, not used currently
 *     Mode is the enum for the interactive with the pipe, USH_PP_MODE_STD is the default
*/
ush_ret_t ush_pipe_create(
    const ush_char_t *pName,         // pipe name
    ush_u64_t         flag,          // bit-mask
    ush_u16_t         timeout,       // the unit is 1s at least, 0 is forever.
    ush_pipe_t       *pPipe);

/*
 * Destory a pipe connecting with the hub
 * ret: OK FAILED WRONG_PARAM WRONG_SEQ
 * restriction: handle should be not NULL, Non-thread-safe
 * detail: handle be delete, invalid pipe leads UB
*/
ush_ret_t ush_pipe_delete(ush_pipe_t pipe);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // USH_PIPE_PUB_H
