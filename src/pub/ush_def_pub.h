#ifndef USH_DEF_PUB_H
#define USH_DEF_PUB_H

#ifdef __cplusplus
extern "C" {
#endif

#define USH_INVALID_PIPE               (0)

#define USH_SIGREG_CONF_MAX    (6)


#ifndef NULL // conflict with the original type
#define NULL ((ush_vptr_t)0)
#endif

#ifdef __cplusplus
}
#endif

#endif // USH_DEF_PUB_H
