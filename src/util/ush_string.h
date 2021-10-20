#ifndef USH_STRING_H
#define USH_STRING_H

#include "stddef.h"

void ush_itoa(char *str, long num);

// conbine a string to a num
void ush_string_certname(char *certname, size_t sz, const char *name, int cert);

#endif // USH_STRING_H
