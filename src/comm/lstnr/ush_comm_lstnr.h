#ifndef USH_COMM_LSTNR_H
#define USH_COMM_LSTNR_H

#include "ush_type_pub.h"

#include "ush_comm_def.h"
#include "ush_comm_desc.h"


typedef enum USH_COMM_LSTNR_MSG_CATALOG {
    USH_COMM_LSTNR_MSG_CATALOG_HAY,
    USH_COMM_LSTNR_MSG_CATALOG_SIGREG_ACK,
    USH_COMM_LSTNR_MSG_CATALOG_SIG_UPD,
    USH_COMM_LSTNR_MSG_CATALOG_MAX
} USH_COMM_LSTNR_MSG_CATALOG;

typedef struct lstnr_msg_desc {
    ush_comm_d                  desc;
    USH_COMM_LSTNR_MSG_CATALOG  catalog;
} USH_COMM_MSG_PACKED ush_comm_lstnr_msg_d;


#endif // USH_COMM_LSTNR_H
