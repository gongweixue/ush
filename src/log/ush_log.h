#ifndef USH_LOG_H
#define USH_LOG_H

#include "stdio.h"

#define ush_log(lvl, ...)   {            \
    printf("\n***** USH "#lvl": \n\t");  \
    printf("File:%s\n\t", __FILE__);     \
    printf("Func:%s\n\t", __FUNCTION__); \
    printf("Line:%d\n\t", __LINE__);     \
    printf(__VA_ARGS__);                 \
    printf("\n");                        \
}

#endif // USH_LOG_H
