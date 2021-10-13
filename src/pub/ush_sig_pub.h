#ifndef USH_SIG_PUB_H
#define USH_SIG_PUB_H

#include "ush_sig_enum.h"
#include "ush_type_pub.h"


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Binding CB for events
 * sync call: n
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM
 * restriction: every new binding will be performed
*/

// when register done
typedef ush_ret_t (*ush_sig_cb_reg_t)(ush_pipe_t         pipe,    // pipe handle
                                      ush_sig_id_t       id,      // signal id
                                      ush_bool_t         success, // done flag
                                      const ush_pvoid_t *pParams);// reserved

// when signal receive
typedef ush_ret_t (*ush_sig_cb_rcv_t)(ush_pipe_t         pipe,    // pipe handle
                                      ush_u32_t          id,      // signal id
                                      const ush_pvoid_t  pVal,    // data
                                      const ush_pvoid_t *pParams);// reserved

typedef struct ush_sig_reg_conf_t {
    ush_sig_id_t     sigid;
    ush_sig_cb_reg_t done;
    ush_sig_cb_rcv_t rcv;
} ush_sig_reg_conf_t;

ush_ret_t ush_sig_reg(ush_pipe_t pipe, const ush_sig_reg_conf_t *pconf);


// implement with burst reg mode
// typedef ush_ret_t *ush_cb_sig_lst_reg_done_t(ush_pipe_t pipe,
//                                              const ush_u32_t *pFailList,
//                                              ush_size_t cnt,
//                                              const ush_pvoid_t *pParams);



/*
 * Get a signal value from service.
 * sync call: y
 * thread safe: n
 * ret: OK FAILED WRONG_PARAM NO_DATA WRONG_SEQ TIMEOUT
 * restriction: value may be out of date when arrived.
 *     0 of Timeout means block for ever.
*/
// ush_ret_t ush_sig_query(ush_pipe_t pipe,
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

#endif // USH_SIG_PUB_H
