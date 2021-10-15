#ifndef USH_COMM_LSTNR_SIGREG_ACK_H
#define USH_COMM_LSTNR_SIGREG_ACK_H

#include "ush_cb_pub.h"

typedef struct lstnr_sigreg_ack * ush_comm_lstnr_sigreg_ack_t;

ush_comm_lstnr_sigreg_ack_t
ush_comm_lstnr_sigreg_ack_create(ush_bool_t   success,
                                 ush_sig_id_t sigid,
                                 ush_pipe_t   pipe,
                                 ush_pvoid_t  done);

ush_size_t
ush_comm_lstnr_sigreg_ack_sizeof();

ush_sig_cb_reg_t
ush_comm_lstnr_sigreg_ack_get_done(const ush_comm_lstnr_sigreg_ack_t msg);

ush_pipe_t
ush_comm_lstnr_sigreg_ack_get_pipe(const ush_comm_lstnr_sigreg_ack_t msg);

ush_sig_id_t
ush_comm_lstnr_sigreg_ack_get_sigid(const ush_comm_lstnr_sigreg_ack_t msg);

ush_bool_t
ush_comm_lstnr_sigreg_ack_get_success(const ush_comm_lstnr_sigreg_ack_t msg);

ush_ret_t
ush_comm_lstnr_sigreg_ack_destroy(ush_comm_lstnr_sigreg_ack_t *pMsg);

void
ush_comm_lstnr_sigreg_ack_testpoint(const ush_comm_lstnr_sigreg_ack_t msg);


#endif // USH_COMM_LSTNR_SIGREG_ACK_H
