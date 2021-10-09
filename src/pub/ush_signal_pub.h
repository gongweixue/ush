#ifndef USH_SIGNAL_PUB_H
#define USH_SIGNAL_PUB_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Binding CB for events
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM
 * restriction: every new binding will be performed, use NULL to remove CB
*/
// typedef ush_ret_t *ush_cb_pp_state_chg_t(ush_pp_hdl_t hdl,
//                                          USH_PIPE_STATE st,
//                                          const ush_pvoid_t *pParams);

// typedef ush_ret_t *ush_cb_pp_attr_chg_t(ush_pp_hdl_t hdl,
//                                         USH_PIPE_ATTR attrType,
//                                         const ush_pvoid_t pVal,
//                                         const ush_pvoid_t *pParams);

typedef ush_ret_t *ush_cb_sig_reg_done_t(ush_pp_hdl_t hdl,
                                         ush_u32_t sigId,
                                         ush_bool_t success,
                                         const ush_pvoid_t *pParams);

// implement with burst reg mode
// typedef ush_ret_t *ush_cb_sig_lst_reg_done_t(ush_pp_hdl_t,
//                                              const ush_u32_t *pFailList,
//                                              ush_size_t cnt,
//                                              const ush_pvoid_t *pParams);


/////////////////////////////////////// signal reached /////////////////////////
typedef ush_ret_t *ush_cb_sig_rcv_t(ush_pp_hdl_t hdl,
                                    ush_u32_t sigId,
                                    const ush_pvoid_t pVal,
                                    const ush_pvoid_t *pParams);
/*
 * ADT for sigal-received event register configuration
 * verbose: 0 for a new val trigger only,
 *          and 1 for receiving any value in any cases.
 * timeSlice: indicates how many slices at least a signal event would
 *            be triggered by service,
 * used in verbose mode only, slice unit is in 10ms.
*/
typedef struct ush_sig_reg_conf_t {
    ush_u32_t        sigId;
    ush_cb_sig_rcv_t pFunc;
    ush_bool_t       verbose;
    ush_u8_t         timeSlice;
} ush_sig_reg_conf_t;
ush_ret_t ush_register_sig_rcv(ush_pp_hdl_t hdl,
                               ush_cb_sig_rcv_t pFunc,
                               const ush_sig_reg_conf_t *pConf);



/*
 * Get a signal value from service.
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM NO_DATA WRONG_SEQ TIMEOUT
 * restriction: value may be out of date when arrived.
 *     0 of Timeout means block for ever.
*/
// ush_ret_t ush_sig_query(ush_pp_hdl_t hdl,
//                         ush_u32_t sigId,
//                         ush_pvoid_t pVal,
//                         ush_u16_t msTimeout);

/*
 * Todo:
 * Is this really nesessary?
*/
// ush_ret_t ush_sig_send();


// ush_ret_t ush_bind_convert_func(ush_id_convert_func_t pFunc);



#ifdef __cplusplus
}
#endif

#endif // USH_SIGNAL_PUB_H
