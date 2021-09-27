
#include "string.h"

#include "ush_string.h"


void ush_itoa(char *str, int num) {
    char *beg = str;
    int sign;
    if ((sign = num) < 0) num = -num;

    do {
        *str++ = '0' + num % 10;
    } while((num /= 10) > 0);

    if (sign < 0) *str++ = '-';

    *str = '\0';

    char *end = str - 1;
    while (beg < end) {
        char tmp = *beg;
        *beg++   = *end;
        *end--   =  tmp;
    }
}


void ush_string_certname(char *certname, const char *name, int cert) {
    strcpy(certname, name);
    strcat(certname, "-");
    char certstr[16];
    ush_itoa(certstr, cert);
    strcat(certname, certstr);
}
