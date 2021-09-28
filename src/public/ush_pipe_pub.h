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

/* state of the pipe */
typedef enum USH_PIPE_STATE {
    USH_PP_STATE_NEW       = 0,
    USH_PP_STATE_OPENED    = 1,
    USH_PP_STATE_JAM       = 2,
    USH_PP_STATE_CLOSED    = 3,
    USH_PP_STATE_NOT_EXIST = 4,
} USH_PIPE_STATE;


typedef enum USH_PIPE_MODE {
    USH_PP_MODE_STD   = 0,
    USH_PP_MODE_BUNCH = 1, // **DO NOT** USE CURRENTLY!!!
    USH_PP_MODE_MAX_GUARD,
} USH_PIPE_MODE;

////////////////////////////////////////////////////////////////////////////////
// Life-time of the pipe:
// NEW -(start)->OPENED -(query/update)->JAM --(stop)->CLOSED ------
//  ^               ^                                   |          |
//  |(@create)      |_________________(start again)_____|          |
//  |                                                              |
// NOT_EXIST <------------------(delete)---------------------------|
////////////////////////////////////////////////////////////////////////////////

/*
 * Create a pipe with hub service
 * sync call: y
 * thread safe: n
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
    ush_vptr_t       *pParams,       // 2-level ptrs for all params, conf-file, etc.
    ush_size_t        paramCnt,
    ush_pp_hdl_t     *pHdl);

/*
 * Destory a pipe connection with the hub
 * sync call: n
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM WRONG_SEQ
 * restriction: handle should be not NULL
 * detail: handle be delete with the indicated time by ms.
 *         Return immediately after the request sending out of the client,
 *         **And all the data will be not perform after the return.
 *         You should use 'query' operation to realize the final result,
 *         e.g. USH_PP_STATE_NOT_EXIST/USH_PP_STATE_CLOSED.
 *         The pipe will also be closed even you don't 'CLOSE' it explicite.
 *         It would be closed by the hub implicite
 *         The flush will be performed when the value is not 0.
*/
ush_ret_t ush_pipe_delete(ush_pp_hdl_t hdl,
                          ush_bool_t   flush,
                          ush_u16_t    msDelay);


/*
 * Flush the data already in the pipe before you deleted/stopped the pipe.
 * sync call: n
 * thread safe: y
 * ret: OK FAILED WRONG_PARAM WRONG_SEQ
 * detail: flush all the data(triger the events) in the indicated pipe.
 * this call may trigger the event 'sig receive'
*/
ush_ret_t ush_pipe_flush(ush_pp_hdl_t hdl);


/*
 * Make the pipe work or not.
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM WRONG_SEQ
 * restriction: not ensure start in the different threads,
 *     in other words, you should always start/stop the same pipe in the same thread.
 *     Data will be not perform after stopping the pipe,
 *     **Use "flush" to clean the data in the pipe manully if you need it.
 *     The flush will be performed when the value is not 0.
*/
ush_ret_t ush_pipe_start(ush_pp_hdl_t hdl);

ush_ret_t ush_pipe_stop(ush_pp_hdl_t hdl, ush_u8_t flush, ush_u16_t msTimeout);


/*
 * Checkout the attr value of a pipe.
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM NOT_SUPPORT NO_DATA
*/
ush_ret_t ush_pipe_query(ush_pp_hdl_t hdl,
                         USH_PIPE_ATTR attr,
                         ush_vptr_t ptr);

/*
 * Update the attr value of a pipe.
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM NOT_SUPPORT WRONG_SEQ
 * restriction: wrong seq once the pipe starts,
 *     need to stop the pipe to update the attr of a pipe
*/
ush_ret_t ush_pipe_update(ush_pp_hdl_t hdl,
                          USH_PIPE_ATTR attr,
                          const ush_vptr_t ptr);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // USH_PIPE_PUB_H
