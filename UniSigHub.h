#ifndef _UNISIGHUB_H_
#define _UNISIGHUB_H_

/*******************************************************************************
 * Header file of the lib for the feature of signal router -- Uni-Signal-Hub
 *
 * **Sync call: wait until the real reslt returns.
 * **Thread safe: it can not be ensure that the call return the correct
 * **result if you use the same context in other thread at the same time.
 * **Keep smart when use CB function if you use C++ as the client language.
 *
 * History:
 * 2021.08.30 Initial version
 * 2021.08.31 Fix some typo, add function register for msg converting.
*******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned char      ush_u8_t;
typedef signed char        ush_s8_t;
typedef unsigned short     ush_u16_t;
typedef signed short       ush_s16_t;
typedef unsigned int       ush_u32_t;
typedef signed int         ush_s32_t;
typedef unsigned long long ush_u64_t;
typedef signed long long   ush_s64_t;
typedef float              ush_fp32_t;
typedef double             ush_fp64_t;
typedef ush_s32_t          ush_pp_hdl_t;
typedef void *             ush_vptr_t;
typedef char               ush_char_t;
typedef unsigned int       ush_size_t;
typedef ush_u8_t           ush_bool_t;


typedef enum {
    USH_CONTENT_TY_STR = 0,
    USH_CONTENT_TY_S8,
    USH_CONTENT_TY_U8,
    USH_CONTENT_TY_S16,
    USH_CONTENT_TY_U16,
    USH_CONTENT_TY_S32,
    USH_CONTENT_TY_U32,
    USH_CONTENT_TY_S64,
    USH_CONTENT_TY_U64,
    USH_CONTENT_TY_FLT,
    USH_CONTENT_TY_DBL,
    USH_CONTENT_TY_RAW,
    USH_CONTENT_TY_ADDR,
} ush_sig_data_type_t;

typedef enum {
    USH_RET_OK          = 0x00,
    USH_RET_FAILED      = 0x01,
    USH_RET_WRONG_PARAM = 0x10, // 8 bits, the low 4 bits for details extention.
    USH_RET_WRONG_SEQ   = 0x20, // wrong sequence on the pipe state-changing
    USH_RET_NO_DATA     = 0x30,
    USH_RET_NOT_SUPPORT = 0x40,
    USH_RET_TIMEOUT     = 0x50,
} ush_ret_t;


/* Versioning for compatible*/
typedef enum {
    USH_VER_OP_MAJOR = 0,
    USH_VER_OP_MINOR,
    USH_VER_OP_ALL,
    USH_VER_OP_MINOR_COMPATIBLE,
} ush_version_op_t;


/* PP type */
typedef enum {
    USH_PIPE_TY_CLUSTER = 0,
    USH_PIPE_TY_HUD     = 1,
    USH_PIPE_TY_ARHUD   = 2,
} ush_pp_type_t;

/* Attr for pipe */
typedef enum {
    USH_PP_ATTR_NAME    = 0,
    USH_PP_ATTR_STATE   = 1,
    USH_PP_ATTR_TYPE    = 2,
    USH_PP_ATTR_MODE    = 3,
    USH_PP_ATTR_FLAG    = 4, // trace, wdg, log, statistics, etc.
} ush_pp_attr_t;

/* state of the pipe */
typedef enum {
    USH_PP_STATE_NEW       = 0,
    USH_PP_STATE_OPENED    = 1,
    USH_PP_STATE_JAM       = 2,
    USH_PP_STATE_CLOSED    = 3,
    USH_PP_STATE_NOT_EXIST = 4,
} ush_pp_state_t;


typedef enum {
    USH_PP_MODE_STD   = 0,
    USH_PP_MODE_BUNCH = 1, // **DO NOT** USE CURRENTLY!!!
} ush_pp_mode_t;

/////////////////////////////////////////////////////////////////////////////////////////////
// Life-time of the pipe:
// -(create)->NEW -(start)->OPENED -(query/update)->JAM -(stop)->CLOSED -(delete)-> NOT_EXIST
//                           |  ^                                   |                 ^
//                           |  |_________________(start again)_____|                 |
//                           |__________________________(delete)______________________|
/////////////////////////////////////////////////////////////////////////////////////////////



/*
 * Get version infomation for confirm the compatiblity with the early version.
 * sync call: y
 * thread safe: y
 * ret: OK FAILED TIMEOUT
 * restriction: pVer should not NULL
 * note: implement with high performance posix or any high performance way
 * detail:
 *     wait until timeout value reached, 0 means forever.
*/
ush_ret_t ush_version_get(ush_version_op_t op, ush_u16_t *pVer, ush_u16_t msTimeout);


/*
 * Create a pipe with hub service
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM WRONG_SEQ NO_DATA TIMEOUT
 * restriction: pName/pHdl should not NULL, same name will return WRONG_PARAM
 * detail:
 *     wait until timeout value reached, 0 means forever. Handle could be passed
 *     to other threads for registering or querying operations, starting, stopping,
 *     but not destructing. The hub deals the request with the arrived order.
 *     Flag is bitmask data, not used currently, default is 0.
 *     Mode is the enum for the interactive with the pipe, USH_PP_MODE_STD is the default
*/
ush_ret_t ush_pipe_create(
    ush_pp_type_t ty,
    const ush_char_t *pName,         // pipe name
    ush_pp_mode_t     mode,          // std / bunch
    ush_u32_t         flag,          // bit-mask for indicating tracing/wdg/logging/statistics and so on.
    ush_u16_t         msTimeout,     // the unit is 1s at least, and 0 is forever.
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
ush_ret_t ush_pipe_delete(ush_pp_hdl_t hdl, ush_u8_t flush, ush_u16_t msDelay);


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
 * restriction: ush_s8_t for ptr of NAME, ush_u32_t for flag, and ush_s32_t for others.
*/
ush_ret_t ush_pipe_query(ush_pp_hdl_t hdl, ush_pp_attr_t attr, ush_vptr_t ptr);

/*
 * Update the attr value of a pipe.
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM NOT_SUPPORT
 * restriction: ush_s8_t for ptr of NAME, ush_u32_t for flag, and ush_s32_t for others.
*/
ush_ret_t ush_pipe_update(ush_pp_hdl_t hdl, ush_pp_attr_t attr, const ush_vptr_t ptr);

/*
 * Find a handle by name.
 * sync call: y
 * thread safe: n (thread sync must be made by yourself)
 * ret: OK FAILED WRONG_PARAM NOT_SUPPORT NO_DATA
 * restriction: sync operation needed for protecting the name changed. Do not use if not nesessary.
*/
ush_ret_t ush_pipe_find(const ush_char_t *pName, ush_pp_hdl_t *pHdl);

/* Events */
typedef enum {
    USH_EVENT_TY_PP_STATE     = 0x1000, // use the high 4bits, mask with others.
    USH_EVENT_TY_PP_ATTR      = 0x2000,
    USH_EVENT_TY_SIG_REG      = 0x3000,
    USH_EVENT_TY_SIG_LIST_REG = 0x3100,
    USH_EVENT_TY_SIG_RCV      = 0x4000,
} ush_event_type_t;


/*
 * Binding CB for events
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM
 * restriction: every new binding will be performed, use NULL to remove CB
 * Todo: callback function list(maybe not needed)
*/
////////////////////////////////////// pipe state changed ////////////////////////////////////////
typedef ush_ret_t *ush_cb_pp_state_chg_t(ush_pp_hdl_t hdl,
                                         ush_pp_state_t st,
                                         const ush_vptr_t *pParams);
ush_ret_t ush_bind_pp_state_chg(ush_pp_hdl_t hdl, ush_cb_pp_state_chg_t pFunc);


////////////////////////////////// pipe attr value changed ///////////////////////////////////////
typedef ush_ret_t *ush_cb_pp_attr_chg_t(ush_pp_hdl_t hdl,
                                        ush_pp_attr_t attrType,
                                        const ush_vptr_t pVal,
                                        const ush_vptr_t *pParams);
ush_ret_t ush_bind_pp_attr_chg(ush_pp_hdl_t hdl, ush_cb_pp_attr_chg_t pFunc);


/////////////////// signal register operation finished on service(really needed?)/////////////////
typedef ush_ret_t *ush_cb_sig_reg_done_t(ush_pp_hdl_t hdl,
                                         ush_u32_t sigId,
                                         ush_bool_t success,
                                         const ush_vptr_t *pParams);
ush_ret_t ush_bind_sig_reg_done(ush_pp_hdl_t hdl, ush_cb_sig_reg_done_t pFunc);



/////////////// signals in the list register operation finished.(really needed?)///////////////////
typedef ush_ret_t *ush_cb_sig_lst_reg_done_t(ush_pp_hdl_t,
                                             const ush_u32_t *pFailList,
                                             ush_size_t cnt,
                                             const ush_vptr_t *pParams);
ush_ret_t ush_bind_sig_list_reg_done(ush_pp_hdl_t hdl, ush_cb_sig_lst_reg_done_t pFunc);


/////////////////////////////////////// signal reached ////////////////////////////////////////////
typedef ush_ret_t *ush_cb_sig_rcv_t(ush_pp_hdl_t hdl,
                                    ush_u32_t sigId,
                                    const ush_vptr_t pVal,
                                    const ush_vptr_t *pParams);
/*
 * ADT for sigal-received event register configuration
 * verbose: 0 for a new val trigger only, and 1 for receiving any value in any cases.
 * timeSlice: indicates how many slices at least a signal event would be triggered by service,
 * used in verbose mode only, slice unit is in 10ms.
*/
typedef struct{
    ush_u32_t        sigId;
    ush_cb_sig_rcv_t pFunc;
    ush_bool_t       verbose;
    ush_u8_t         timeSlice;
} ush_sig_reg_conf_t;
ush_ret_t ush_register_sig_rcv(ush_pp_hdl_t hdl, ush_cb_sig_rcv_t pFunc, const ush_sig_reg_conf_t *pConf);



/*
 * Get a signal value from service.
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM NO_DATA WRONG_SEQ TIMEOUT
 * restriction: value may be out of date when arrived. 0 of Timeout means block for ever.
*/
ush_ret_t ush_sig_query(ush_pp_hdl_t hdl, ush_u32_t sigId, ush_vptr_t pVal, ush_u16_t msTimeout);

/*
 * Todo:
 * Is this really nesessary?
*/
// ush_ret_t ush_sig_send();


/*
 * This function will be invoked by the client lib when it should be, e.g. get content size
 * sync: y
 * thread safe: y and can be re-entry
 * detail: this function's provider must be ensure that the behavior correctly on converting
 * between id and name(e.g. dealing NULL and note modify the input parameter).
 * 'dir' is for converting direction.
 * Considering the relation is not linear, the provider must make sure the performance
 * of the function not too bad(it's running in your own thread). Find an ADT for both dirs.
 * The client lib may or may not call it, depends on the condition or the performance.
 * You can also use it in your local code at any time you want since it could be re-entried.
*/
typedef ush_u32_t (*ush_id_convert_func_t)(ush_u32_t  *pId,
                                           ush_char_t *pName,
                                           ush_size_t *pDataSz,
                                           ush_bool_t  dir);
ush_ret_t ush_bind_convert_func(ush_id_convert_func_t pFunc);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // _UNISIGHUB_H_
