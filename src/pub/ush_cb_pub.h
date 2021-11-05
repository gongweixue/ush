#ifndef USH_CB_PUB_H
#define USH_CB_PUB_H

#include "ush_type_pub.h"
#include "ush_sig_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Binding CB for signal registering & signal value updating
 * ret: OK FAILED WRONG_PARAM
 * restriction: every new binding will be performed
*/
// when register done
typedef ush_ret_t (*ush_sig_cb_reg_t)(ush_pipe_t         pipe,    // pipe handle
                                      ush_sig_id_t       id,      // signal id
                                      ush_bool_t         success);


// when signal receive
typedef ush_ret_t (*ush_sig_cb_rcv_t)(ush_sig_id_t  id,
                                      ush_sig_val_t val,
                                      ush_u32_t     cntr);



#ifdef __cplusplus
}
#endif


#endif // USH_CB_PUB_H
