#include "assert.h"
#include "string.h"

#include "ush_string.h"


void ush_itoa(char *str, long num) {
    static const long base = 10;
    char *beg = str;
    long sign;
    if ((sign = num) < 0) {num = -num;}

    do {
        *str++ = (char)('0' + num % base);
    } while((num /= base) > 0);

    if (sign < 0) {*str++ = '-';}

    *str = '\0';

    char *end = str - 1;
    while (beg < end) {
        char tmp = *beg;
        *beg++   = *end;
        *end--   =  tmp;
    }
}

#define USH_COMM_LSTNR_Q_PATH_PREFIX "/USH-LSTNR-"
// [/USH-LSTNR-][shortname][-timestamp-][cert]
//    11B           20B         11B       10B
void ush_string_gen_lstnr_fullname(char *dst, size_t dstsz,
                                   const char *shortname_ts,
                                   int cert) {
    assert(dst && shortname_ts);

    char certstr[16];
    ush_itoa(certstr, cert);

    size_t total = strlen(USH_COMM_LSTNR_Q_PATH_PREFIX)
                 + strlen(shortname_ts)
                 + 1
                 + strlen(certstr);
    assert(dstsz > total);

    strncpy(dst, USH_COMM_LSTNR_Q_PATH_PREFIX, dstsz);
    strncat(dst, shortname_ts, 20+1+10);
    strncat(dst, "-", 1);
    strncat(dst, certstr, 10);
}

#define USH_COMM_REALM_Q_PATH_PREFIX "/USH-REALM-"
// [/USH-REALM-][shortname][-timestamp-][cert]
//    11B           20B         11B       10B
void ush_string_gen_realm_fullname(char *dst, size_t dstsz,
                                   const char *shortname_ts,
                                   int cert) {
    assert(dst && shortname_ts);

    char certstr[16];
    ush_itoa(certstr, cert);

    size_t total = strlen(USH_COMM_REALM_Q_PATH_PREFIX)
                 + strlen(shortname_ts)
                 + 1
                 + strlen(certstr);
    assert(dstsz > total);

    strncpy(dst, USH_COMM_REALM_Q_PATH_PREFIX, dstsz);
    strncat(dst, shortname_ts, 20+1+10);
    strncat(dst, "-", 1);
    strncat(dst, certstr, 10);
}
