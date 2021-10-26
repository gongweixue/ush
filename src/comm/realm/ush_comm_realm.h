#ifndef USH_COMM_REALM_H
#define USH_COMM_REALM_H

#include "ush_type_pub.h"

#include "ush_comm_def.h"
#include "ush_comm_desc.h"

typedef enum USH_COMM_REALM_MSG_CATALOG {
    USH_COMM_TCH_MSG_CATALOG_SIG,
    USH_COMM_TCH_MSG_CATALOG_MAX
} USH_COMM_REALM_MSG_CATALOG;

typedef struct comm_realm_msg_desc {
    ush_comm_d                    desc;
    USH_COMM_REALM_MSG_CATALOG    catalog;
} USH_COMM_MSG_PACKED ush_comm_realm_msg_d;



#endif // USH_COMM_REALM_H
