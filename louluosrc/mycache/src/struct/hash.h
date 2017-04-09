#ifndef _HASH_H
#define _HASH_H
#include "cdef.h"

#ifdef __cplusplus
extern "C" {
#endif


void crypt_table_init(void);
ulong_t hash_string(const char *cstring, ulong_t stringlen, int  hash_type);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_HASH_H




