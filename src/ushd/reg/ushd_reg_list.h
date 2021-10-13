#ifndef USHD_REG_LIST_H
#define USHD_REG_LIST_H

#include "ush_type_pub.h"

typedef struct ushd_reg_list * ushd_reg_list_t;

ushd_reg_list_t ushd_reg_list_create();

// @@@@@@@@@@@@@@@ accessing reg list must be entry critical section !!!!

#endif // USHD_REG_LIST_H
