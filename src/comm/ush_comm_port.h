#ifndef USH_COMM_PORT_H
#define USH_COMM_PORT_H

#include "ush_comm_def.h"
#include "ush_type_pub.h"

typedef enum USH_COMM_PORT {
    USH_COMM_PORT_TCH,
    USH_COMM_PORT_LSTNR,
    USH_COMM_PORT_MAX
} USH_COMM_PORT;

typedef struct comm_port_desc {
    USH_COMM_PORT  port;
} USH_COMM_MSG_PACKED ush_comm_port_d;


#endif // USH_COMM_PORT_H
