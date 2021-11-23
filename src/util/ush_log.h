#ifndef USH_LOG_H
#define USH_LOG_H

#include "stdio.h"

typedef enum USH_LOG_LVL {
    LOG_LVL_FATAL,  // routine exit
    LOG_LVL_ERROR,  // function return
    LOG_LVL_INFO,   // code block finished
} USH_LOG_LVL;

#ifndef USH_LOG_ON
    #define USH_LOG_ON              (1)
#endif

#ifndef USH_LOG_LVL_SELECTOR
    #define USH_LOG_LVL_SELECTOR    LOG_LVL_ERROR
#endif

void ush_log_cs_entry(void);
void ush_log_cs_exit(void);

#if !USH_LOG_ON
    #define log_def(owner, lvl, ...)
#else
    #define log_def(owner, lvl, ...)   do {                                     \
        ush_log_cs_entry();                                                     \
        if (lvl <= USH_LOG_LVL_SELECTOR) {                                      \
            printf("*****" #owner"-"#lvl":\n");                                 \
            printf("---- At: %s:%d %s\n", __FILE__, __LINE__, __FUNCTION__);    \
            printf("---- "); printf(__VA_ARGS__);                               \
            printf("\n\n");                                                     \
        }                                                                       \
        ush_log_cs_exit();                                                      \
    } while (0)
#endif // !USH_LOG_ON


#define ush_log(lvl, ...) do { log_def(USH, lvl, __VA_ARGS__);} while(0)


#define ushd_log(lvl, ...) do { log_def(USHD, lvl, __VA_ARGS__);} while(0)

#endif // USH_LOG_H
