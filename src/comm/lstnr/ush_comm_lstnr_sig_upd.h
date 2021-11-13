#ifndef USH_COMM_LSTNR_SIG_UPD_H
#define USH_COMM_LSTNR_SIG_UPD_H

#include "ush_cb_pub.h"

typedef struct lstnr_sig_upd_s * ush_comm_lstnr_sig_upd_t;

ush_comm_lstnr_sig_upd_t
ush_comm_lstnr_sig_upd_create(ush_sigid_t   sigid,
                              ush_sig_val_t val,
                              ush_pvoid_t   rcv,
                              ush_u32_t     cntr);

ush_size_t
ush_comm_lstnr_sig_upd_sizeof(void);

ush_sig_cb_rcv_t
ush_comm_lstnr_sig_callback_of(const ush_comm_lstnr_sig_upd_t msg);

ush_sig_val_t
ush_comm_lstnr_sig_value_of(const ush_comm_lstnr_sig_upd_t msg);

ush_sigid_t
ush_comm_lstnr_sig_sigid_of(const ush_comm_lstnr_sig_upd_t msg);

ush_u32_t
ush_comm_lstnr_sig_cntr_of(const ush_comm_lstnr_sig_upd_t msg);

ush_ret_t
ush_comm_lstnr_sig_upd_destroy(ush_comm_lstnr_sig_upd_t *pMsg);

void
ush_comm_lstnr_sig_upd_testpoint(const ush_comm_lstnr_sig_upd_t msg);


#endif // USH_COMM_LSTNR_SIG_UPD_H
