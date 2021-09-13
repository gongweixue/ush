#ifndef USH_SIGNAL_PUB_H
#define USH_SIGNAL_PUB_H

#ifdef __cplusplus
extern "C" {
#endif


typedef enum ush_sig_data_type_t {
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



/* Events */
typedef enum ush_event_type_t {
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
typedef struct ush_sig_reg_conf_t {
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


ush_ret_t ush_bind_convert_func(ush_id_convert_func_t pFunc);



#ifdef __cplusplus
}
#endif

#endif // USH_SIGNAL_PUB_H
