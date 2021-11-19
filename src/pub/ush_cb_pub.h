#ifndef USH_CB_PUB_H
#define USH_CB_PUB_H

#include "ush_type_pub.h"
#include "ush_sigid_pub.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Binding CB for signal registering & signal value updating
 * ret: OK FAILED WRONG_PARAM
 * restriction: Every new binding will be performed.
 *              You can register USH_SIGREG_CONF_MAX sigid every single time.
 *              Be clear that how many signals had been registered,
 *              and get them from pSigids and pSucc.
 *
 *              Every new value different with the old one will be performed.
 *              And the value of 'ver' is the fingerprint and incresed by 1
 *              every time changed, use it as you need.
*/
// when register done
typedef ush_ret_t (*ush_sig_cb_reg_t)(ush_pipe_t         pipe,    // pipe handle
                                      const ush_sigid_t *pSigids, // signal id
                                      const ush_bool_t  *pSucc);


// when signal receive
typedef ush_ret_t (*ush_sig_cb_rcv_t)(ush_sigid_t   id,
                                      ush_sig_val_t val,
                                      ush_u32_t     ver);



#ifdef __cplusplus
}
#endif

#endif // USH_CB_PUB_H
