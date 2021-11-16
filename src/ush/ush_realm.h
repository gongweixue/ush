#ifndef USH_REALM_H
#define USH_REALM_H

#include "ush_type_pub.h"
#include "ush_comm_realm.h"

typedef struct ush_realm_s * ush_realm_t;

ush_ret_t ush_realm_alloc(ush_realm_t *pRealm, const ush_char_t *fullname);

ush_ret_t ush_realm_open(ush_realm_t realm);

ush_ret_t ush_realm_send(ush_realm_t realm, const ush_comm_realm_msg_d *msg);

ush_ret_t ush_realm_destroy_with_closing(ush_realm_t *pRealm);
#endif // USH_REALM_H
