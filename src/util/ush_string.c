
#include "ush_string.h"


void ush_itoa(int a, char *str) {
    char *beg = str;
    int sign;
    if ((sign = a) < 0) a = -a;

    do {
        *str++ = '0' + a % 10;
    } while((a /= 10) > 0);

    if (sign < 0) *str++ = '-';

    *str = '\0';

    char *end = str - 1;
    while (beg < end) {
        char tmp = *beg;
        *beg++   = *end;
        *end--   =  tmp;
    }
}
