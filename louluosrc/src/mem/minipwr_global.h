#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define nx_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *) 0)->MEMBER)

#define nx_container_of(ptr, type, member) ({                      \
        const typeof(((type *) 0)->member) *__mptr = (ptr);     \
        (type *) ((char *) __mptr - nx_offsetof(type, member));})

#define MIN_ALIGNMENT 8
#define ALLOC_ALIGNMENT (MIN_ALIGNMENT<<1)
