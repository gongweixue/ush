#ifndef USH_LOG_H
#define USH_LOG_H

#include "stdio.h"

typedef enum USH_LOG_LVL {
    LOG_LVL_FATAL,
    LOG_LVL_ERROR,
    LOG_LVL_INFO,
    LOG_LVL_DETAIL,
    LOG_LVL_VERBOSE
} USH_LOG_LVL;


#define log_def(owner, lvl, ...)   {            \
    printf("\n*****" #owner"-"#lvl": \n\t");  \
    printf("File:%s\n\t", __FILE__);     \
    printf("Func:%s\n\t", __FUNCTION__); \
    printf("Line:%d\n\t", __LINE__);     \
    printf(__VA_ARGS__);                 \
    printf("\n");                        \
}

#define ush_log(lvl, ...) log_def(USH, lvl, __VA_ARGS__)

#define ushd_log(lvl, ...) log_def(USHD, lvl, __VA_ARGS__)

#endif // USH_LOG_H
