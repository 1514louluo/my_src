#ifndef _CDEF_H
#define _CDEF_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include<ctype.h>


#include <pthread.h>

#include <unistd.h>
#include <sys/time.h>

#include <sys/resource.h>

typedef unsigned long ulong_t;
typedef long long_t;

#define P_MALLOC malloc
#define P_FREE   free

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *) 0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({                      \
        const typeof(((type *) 0)->member) *__mptr = (ptr);     \
        (type *) ((char *) __mptr - offsetof(type, member));})
#endif

#define MIN_ALIGNMENT 8
#ifdef _CPU_32BIT
#define ALLOC_ALIGNMENT  MIN_ALIGNMENT
#else
#define ALLOC_ALIGNMENT (MIN_ALIGNMENT<<1)
#endif

#endif//_CDEF_H

