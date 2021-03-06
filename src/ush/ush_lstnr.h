#ifndef USH_LSTNR_H
#define USH_LSTNR_H


typedef struct ush_lstnr_s * ush_lstnr_t;


#include "ush_type_pub.h"


ush_ret_t
ush_lstnr_open_start(ush_lstnr_t *pPtr, const ush_char_t *fullname);

ush_ret_t
ush_lstnr_stop_close(ush_lstnr_t *pPtr);


#endif // USH_LSTNR_H
