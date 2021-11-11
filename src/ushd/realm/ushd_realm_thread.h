#ifndef USHD_REALM_THREAD_H
#define USHD_REALM_THREAD_H

#include "ush_type_pub.h"

typedef struct ushd_realm_thread_s * ushd_realm_thread_t;

ushd_realm_thread_t
ushd_realm_thread_create(const ush_char_t *name);

ush_ret_t
ushd_realm_thread_start(ushd_realm_thread_t thread);


ush_ret_t
ushd_realm_thread_request_stop(ushd_realm_thread_t *pThread);

#endif // USHD_REALM_THREAD_H
