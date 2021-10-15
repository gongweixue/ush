#ifndef USHD_REG_LIST_H
#define USHD_REG_LIST_H

#include "ush_type_pub.h"

typedef struct ushd_reg_list * ushd_reg_list_t;

ushd_reg_list_t ushd_reg_list_create();

ush_ret_t ushd_reg_list_set_cb_rcv(ushd_reg_list_t reglist,
                                   ush_sig_id_t    sigid,
                                   ush_pvoid_t     rcv);

// @@@@@@@@@@@@@@@ accessing reg list must be entry critical section !!!!

#endif // USHD_REG_LIST_H
