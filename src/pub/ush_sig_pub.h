#ifndef USH_SIG_PUB_H
#define USH_SIG_PUB_H

#include "ush_sigid_pub.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Binding CB for signal registering & signal value updating
 * ret: OK FAILED WRONG_PARAM
 * restriction: every new binding will be performed.
 *              You should be clear how many signals had been registered,
 *              and get them from pSigids and pSucc.
*/
// when register done
typedef ush_ret_t (*ush_sig_cb_reg_t)(ush_pipe_t         pipe,    // pipe handle
                                      const ush_sigid_t *pSigids, // signal id
                                      const ush_bool_t  *pSucc);


// when signal receive
typedef ush_ret_t (*ush_sig_cb_rcv_t)(ush_sigid_t   id,
                                      ush_sig_val_t val,
                                      ush_u32_t     cntr);



#ifndef USH_SIGREG_CONF_MAX
#define USH_SIGREG_CONF_MAX    (6)
#endif

typedef struct ush_sigreg_conf_s {
    ush_sigid_t      sigid[USH_SIGREG_CONF_MAX];
    ush_sig_cb_reg_t done;
    ush_sig_cb_rcv_t rcv[USH_SIGREG_CONF_MAX];
    ush_u32_t        count;
} ush_sigreg_conf_t;


/*
 * Listen a sigid on the ush signal bus by the callback
 * Sync call: n
 * Ret: OK FAILED WRONG_PARAM WRONG_SEQ
 * Usage: params wrong will leads the non-0 return,
 *     the functions will be blocked if the msg not been sent out.
 *     but the return dose not mean the msg value has been set on the ushd.
 *     Pass NULL to reset the callbacks.
 * Caution: Wrong pipe id will lead a UB;
 *          Max num of signals once is USH_SIGREG_CONF_MAX, take case yourself.
 *          Wrong num of the signal will lead UB
 */
ush_ret_t ush_sigreg(ush_pipe_t pipe, const ush_sigreg_conf_t *pConf);

/*
 * Update a signal value to signal bus
 * Sync call: n
 * Ret: OK FAILED WRONG_PARAM WRONG_SEQ
 * Usage: params wrong will leads the non-0 return,
 *     the functions will be blocked if the msg not been sent out.
 *     but the return dose not mean the msg value has been set on the ushd.
 *     Client who listening this sigid will be triggered by the callback.
 * Caution: Wrong pipe id will lead a UB
 */
ush_ret_t ush_sigset(ush_pipe_t    pipe,
                     ush_sigid_t   sigid,
                     ush_sig_val_t value);


/*
 * Request a signal value notify from the pipe asynchronously
 * Sync call: n
 * Ret: OK FAILED WRONG_PARAM WRONG_SEQ
 * Usage: Function call will return immediately when the request send out.
          Before that, you should set the correct callback function you need.
          The call will return non-OK value if the pipe/sigid is invalid.
          Do what you want in the callback.
 * Caution: Wrong pipe id will lead a UB
 */
ush_ret_t ush_sigtease(ush_pipe_t pipe, ush_sigid_t sigid);


// implement with burst reg mode
// typedef ush_ret_t *ush_cb_sig_lst_reg_done_t(ush_pipe_t pipe,
//                                              const ush_u32_t *pFailList,
//                                              ush_size_t cnt,
//                                              const ush_pvoid_t *pParams);

#ifdef __cplusplus
}
#endif

#endif // USH_SIG_PUB_H
