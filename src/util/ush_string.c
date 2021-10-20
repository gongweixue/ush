#include "assert.h"
#include "string.h"

#include "ush_string.h"


void ush_itoa(char *str, long num) {
    char *beg = str;
    long sign;
    if ((sign = num) < 0) {num = -num;}

    do {
        *str++ = (char)('0' + num % 10);
    } while((num /= 10) > 0);

    if (sign < 0) {*str++ = '-';}

    *str = '\0';

    char *end = str - 1;
    while (beg < end) {
        char tmp = *beg;
        *beg++   = *end;
        *end--   =  tmp;
    }
}


void ush_string_certname(char *dst, size_t sz, const char *name, int cert) {
    assert(dst);

    char certstr[16];
    ush_itoa(certstr, cert);
    assert(sz > strlen(name) + 1 + strlen(certstr));

    strcpy(dst, name);
    strcat(dst, "-");
    strcat(dst, certstr);
}
