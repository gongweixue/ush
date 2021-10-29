#ifndef USH_COMM_LSTNR_SIG_UPD_H
#define USH_COMM_LSTNR_SIG_UPD_H

#include "ush_cb_pub.h"

typedef struct lstnr_sig_upd_s * ush_comm_lstnr_sig_upd_t;

ush_comm_lstnr_sig_upd_t
ush_comm_lstnr_sig_upd_create(ush_sig_id_t  sigid,
                              ush_sig_val_t val,
                              ush_pvoid_t   rcv);

ush_size_t
ush_comm_lstnr_sig_upd_sizeof(void);

ush_sig_cb_rcv_t
ush_comm_lstnr_sig_upd_get_rcv(const ush_comm_lstnr_sig_upd_t msg);

ush_sig_val_t
ush_comm_lstnr_sig_upd_get_value(const ush_comm_lstnr_sig_upd_t msg);

ush_sig_id_t
ush_comm_lstnr_sig_upd_get_sigid(const ush_comm_lstnr_sig_upd_t msg);

ush_ret_t
ush_comm_lstnr_sig_upd_destroy(ush_comm_lstnr_sig_upd_t *pMsg);

void
ush_comm_lstnr_sig_upd_testpoint(const ush_comm_lstnr_sig_upd_t msg);


#endif // USH_COMM_LSTNR_SIG_UPD_H
