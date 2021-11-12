#ifndef USH_PORTING_H
#define USH_PORTING_H

#include "time.h"
#include "mqueue.h"

// #define QNX

#ifndef USH_CLOCK_ID
    #ifdef QNX
        #define USH_CLOCK_ID CLOCK_MONOTONIC
    #else
        #define USH_CLOCK_ID CLOCK_REALTIME
    #endif // QNX
#endif // USH_CLOCK_ID

#ifndef USH_MQ_TIMEDSEND
    #ifdef QNX
        #define USH_MQ_TIMEDSEND mq_timedsend_monotonic
    #else
        #define USH_MQ_TIMEDSEND mq_timedsend
    #endif // QNX
#endif // USH_MQ_TIMEDSEND

#define USH_TCH_QUEUE_SENDING_TIMEOUT_SEC (2)
#define USH_DIST_QUEUE_SENDING_TIMEOUT_SEC  (2);
#define USH_REALM_QUEUE_SENDING_TIMEOUT_SEC (2)

#endif // USH_PORTING_H
