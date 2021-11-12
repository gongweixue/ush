#ifndef USH_DEFINE_H
#define USH_DEFINE_H

#include "ush_porting.h"

#define USH_INVALID_CERT_VALUE         (-1)

#define USH_INVALID_TID                (USH_INVALID_THREAD_ID)

#define USH_INVALID_MQD_VALUE          (-1)

#define USH_INVALID_PIPE               (0)

#define USHD_INVALID_CONN_IDX_VALUE    (0)
#define USH_CONN_IDX_MAX               (128)

#define USH_COMM_TCH_SEND_PRIO_HELLO             (0)
#define USH_COMM_TCH_SEND_PRIO_GOODBYE           (1)

#define USH_COMM_REALM_SEND_PRIO_SIGSET           (0)
#define USH_COMM_REALM_SEND_PRIO_SIGTEASE         (0)
#define USH_COMM_REALM_SEND_PRIO_SIGREG           (1)
#define USH_COMM_REALM_SEND_PRIO_CMD              (2)

#define USH_COMM_LSTNR_SEND_PRIO_HAY             (0)
#define USH_COMM_LSTNR_SEND_PRIO_SIGREG_ACK      (0)
#define USH_COMM_LSTNR_SEND_PRIO_SIG_UPD         (1)
#define USH_COMM_LSTNR_SEND_PRIO_STOP            (2)



#define USH_TCH_QUEUE_SENDING_TIMEOUT_SEC   (2)
#define USH_DIST_QUEUE_SENDING_TIMEOUT_SEC  (2)
#define USH_REALM_QUEUE_SENDING_TIMEOUT_SEC (2)

#endif // USH_DEFINE_H
