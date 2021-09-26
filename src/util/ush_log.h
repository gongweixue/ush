#ifndef USH_LOG_H
#define USH_LOG_H

#include "stdio.h"

typedef enum USH_LOG_LVL {
    LOG_LVL_FATAL,  // routine exit
    LOG_LVL_ERROR,  // function return
    LOG_LVL_INFO,   // code block finished
    LOG_LVL_DETAIL  // every step of the routine
} USH_LOG_LVL;


#define USH_LOG_ON              (1)
#define USH_LOG_LVL_SELECTOR    LOG_LVL_ERROR

void ush_log_cs_enter();
void ush_log_cs_exit();

#if !USH_LOG_ON
#define log_def(owner, lvl, ...)
#else
#define log_def(owner, lvl, ...)   {                                           \
    ush_log_cs_enter();                                                        \
    if (lvl <= USH_LOG_LVL_SELECTOR) {                                         \
        printf("*****" #owner"-"#lvl":\n");                                    \
        printf("---- At :%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);       \
        printf("---- "); printf(__VA_ARGS__);                                  \
        printf("\n\n");                                                        \
    }                                                                          \
    ush_log_cs_exit();                                                         \
}
#endif // !USH_LOG_ON


#define ush_log(lvl, ...) log_def(USH, lvl, __VA_ARGS__)


#define ushd_log(lvl, ...) log_def(USHD, lvl, __VA_ARGS__)

#endif // USH_LOG_H
