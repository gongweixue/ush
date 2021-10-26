#ifndef USH_STRING_H
#define USH_STRING_H

#include "stddef.h"

void ush_itoa(char *str, long num);

// conbine a string to a num
void ush_string_gen_lstnr_fullname(char *dst, size_t dstsz,
                                   const char *shortname_ts,
                                   int cert);
void ush_string_gen_realm_fullname(char *dst, size_t dstsz,
                                   const char *shortname_ts,
                                   int cert);

#endif // USH_STRING_H
