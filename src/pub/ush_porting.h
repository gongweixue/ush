#ifndef USH_PORTING_H
#define USH_PORTING_H

#include "time.h"
#include "mqueue.h"

#ifndef USH_CLOCK_ID
    #ifdef __QNXNTO__
        #define USH_CLOCK_ID CLOCK_MONOTONIC // in case of time sync action
    #else
        #define USH_CLOCK_ID CLOCK_REALTIME
    #endif // __QNXNTO__
#endif // USH_CLOCK_ID

#ifndef USH_MQ_TIMEDSEND
    #ifdef __QNXNTO__
        #define USH_MQ_TIMEDSEND mq_timedsend_monotonic
    #else
        #define USH_MQ_TIMEDSEND mq_timedsend
    #endif // __QNXNTO__
#endif // USH_MQ_TIMEDSEND

#endif // USH_PORTING_H
