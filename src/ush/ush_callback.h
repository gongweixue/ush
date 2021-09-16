#ifndef USH_CALLBACK_H
#define USH_CALLBACK_H


#ifdef __cplusplus
extern "C" {
#endif

/*
 * This function will be invoked by the client lib when it should be,
 * e.g. get content size
 *
 * sync: y
 * thread safe: y and can be re-entry
 * detail: this function's provider must be ensure that the behavior correctly
 *         on converting between id and name(e.g. dealing NULL and note modify
 *         the input parameter).
 *
 *         'dir' is for converting direction.
 *         Considering the relation is not linear, the provider must make sure
 *         the performance of the function not too bad(runs in your own thread).
 *
 *         Find an ADT for both dirs.
 *         The client lib may or may not call it, depends on the condition or
 *         the performance. You can also use it in your local code at any time
 *         you want since it could be re-entried.
*/
typedef ush_u32_t (*ush_id_convert_func_t)(ush_u32_t  *pId,
                                           ush_char_t *pName,
                                           ush_size_t *pDataSz,
                                           ush_bool_t  dir);


#ifdef __cplusplus
}
#endif

#endif // USH_CALLBACK_H
