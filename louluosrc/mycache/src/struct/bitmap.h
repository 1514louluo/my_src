#ifndef _BITMAP_H
#define _BITMAP_H
#include "cdef.h"

#define POWER_PER_BYTE       3      /* 2 pow 3 = 8*/
#define POWER_PER_LONG       6      /* 2 pow 3 = 8*/

#define BITS_PER_BYTE        8
#define BITS_PER_LONG        (BITS_PER_BYTE * sizeof(ulong_t))
#define BITS_TO_LONGS(nr)    DIV_ROUND_UP(nr, BITS_PER_LONG)

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define small_const_nbits(nbits) \
	(__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG)

#define BITMAP_LAST_WORD_MASK(nbits)					\
(									\
	((nbits) % BITS_PER_LONG) ?					\
		(1UL<<((nbits) % BITS_PER_LONG))-1 : ~0UL		\
)

#define bitmap_setbit(bm, pos)   bm_setbit(bm, pos)
#define bitmap_clrbit(bm, pos)   bm_clrbit(bm, pos)
#define bitmap_isset(bm, pos)    bm_isset(bm, pos)
#define bitmap_zero(bm, nbits)   bm_zero(bm, nbits)
#define bitmap_fill(bm, nbits)   bm_fill(bm, nbits)

#define bitmap_isset_region(bm, pos, region)    bm_isset_region(bm, pos, region)
#define bitmap_setbit_region(bm, pos, region)   bm_setbit_region(bm, pos, region)
#define bitmap_clrbit_region(bm, pos, region)   bm_clrbit_region(bm, pos, region)

static inline void _bm_getmask(int *pos, ulong_t *bmask)
{
        *bmask = (ulong_t)(1UL << (*pos % BITS_PER_LONG));
        *pos  >>= POWER_PER_LONG;
}

static inline void bm_setbit(ulong_t *bm, int pos)
{
        ulong_t   bmask;

        _bm_getmask(&pos, &bmask);
        bm[pos] |= bmask;
}

static inline void bm_setbit_region(ulong_t *bm, int pos, int region)
{
        int i=0;
        for(; i < region; ++i)
        {
                bm_setbit(bm, pos + i);
        }
}


static inline void bm_clrbit(ulong_t * bm, int pos)
{
        ulong_t   bmask;

        _bm_getmask(&pos, &bmask);
        bm[pos] &= ~bmask;
}

static inline void bm_clrbit_region(ulong_t *bm, int pos, int region)
{
        int i=0;
        for(; i < region; ++i)
        {
                bm_clrbit(bm, pos + i);
        }
}

static inline int bm_isset(ulong_t * bm, int pos)
{
        ulong_t   bmask;

        _bm_getmask(&pos, &bmask);
        return !!(bm[pos] & bmask);
}

static inline int bm_isset_region(ulong_t *bm, int pos, int region)
{
        int i = 0;
        for(; i < region; ++i)
        {
                if(bm_isset(bm, pos + i))
                        return 1;
        }
        return 0;
}

static inline void bm_zero(ulong_t *dst, size_t nbits) //0初始化
{
        if(small_const_nbits(nbits))
                *dst = 0;
        else
        {
                int len = BITS_TO_LONGS(nbits) * sizeof(ulong_t);
                memset(dst, 0, len);
        }
}

static inline void bm_fill(ulong_t *dst, size_t nbits) // 1 初始化
{
        size_t nlongs = BITS_TO_LONGS(nbits);
        if(!small_const_nbits(nbits))
        {
                int len = (nlongs - 1) * sizeof(ulong_t);
                memset(dst, 0xff,  len);
        }
        dst[nlongs - 1] = BITMAP_LAST_WORD_MASK(nbits);
}

/*just for testing!*/
/*
extern void bm_foreach(ulong_t *bmap, int size)
{
        int i=0;
        for(; i < size; ++i)
                printf("%d", bm_isset(bmap, i));
        printf("\n");
}
*/
#endif //_BITMAP_H

