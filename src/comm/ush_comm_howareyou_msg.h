#ifndef USH_COMM_HOWAREYOU_MSG_H
#define USH_COMM_HOWAREYOU_MSG_H

#include "ush_comm_listener.h"

#define USH_COMM_HOWAREYOU_MSG_PRIO         (0)

typedef struct ush_comm_howareyou_msg {
    ush_listener_msg_desc_t desc;
    ush_vptr_t              sync;
    ush_connect_ident       ident;
} ush_comm_howareyou_msg USH_COMM_MSG_ALIGNMENT;


#endif // USH_COMM_HOWAREYOU_MSG_H
