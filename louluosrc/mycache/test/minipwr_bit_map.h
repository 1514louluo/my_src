#ifndef _MINIPWR_BIT_MAP_H
#define _MINIPWR_BIT_MAP_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BITS_PER_BYTE        8
#define BYTES_PER_LONG     sizeof(ulong_t)
#define BITS_PER_LONG        (BITS_PER_BYTE * BYTES_PER_LONG)
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define BITS_TO_LONGS(nr)    DIV_ROUND_UP(nr, BITS_PER_LONG)

#define small_const_nbits(nbits) \
	(__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG)

#define BITMAP_LAST_WORD_MASK(nbits)					\
(									\
	((nbits) % BITS_PER_LONG) ?					\
		(1UL<<((nbits) % BITS_PER_LONG))-1 : ~0UL		\
)

inline void bitmap_zero(unsigned long *dst, int nbits) //0初始化
{
	if (small_const_nbits(nbits))
		*dst = 0;
	else {
		int len = BITS_TO_LONGS(nbits) * BYTES_PER_LONG;
		memset(dst, 0, len);
	}
}

inline void bitmap_fill(unsigned long *dst, int nbits) //1初始化
{
	int nlongs = BITS_TO_LONGS(nbits);
	if (!small_const_nbits(nbits)) {
		int len = (nlongs - 1) * BYTES_PER_LONG;
		memset(dst, 0xff,  len);
	}
	dst[nlongs - 1] = BITMAP_LAST_WORD_MASK(nbits);
}
inline void bitmap_cpl(unsigned long *dst, int nbits) //对某位取反
{
	int index = BITS_TO_LONGS(nbits);
	if(!index) return;
	int pos = nbits % BITS_PER_LONG;
	if(!pos) pos = BITS_PER_LONG;
	dst[index - 1] ^= 1UL<<(pos-1);
}
inline int bitmap_find(unsigned long *dst, int nbits) //查看某位值
{
	int index = BITS_TO_LONGS(nbits);
	if(!index) return 0;
	int pos = nbits % BITS_PER_LONG;
	if(!pos) pos = BITS_PER_LONG;
	return (dst[index - 1] & (1UL<<(pos-1)));
}

static inline int __ffs(uint32_t x)  
{  
    int r = 0;  
  
    if (!x)  
        return 0;  
    if (!(x & 0xffff)) {  
        x >>= 16;  
        r += 16;  
    }  
    if (!(x & 0xff)) {  
        x >>= 8;  
        r += 8;  
    }  
    if (!(x & 0xf)) {  
        x >>= 4;  
        r += 4;  
    }  
    if (!(x & 3)) {  
        x >>= 2;  
        r += 2;  
    }  
    if (!(x & 1)) {  
        x >>= 1;  
        r += 1;  
    }  
    return r;  
}  
static inline int __ffz(uint32_t x)
{
	return __ffs(~x);
}

inline int bitmap_ffs(unsigned long *dst, int nb) //查找第一位为一的位置
{
	int i=0;
	for(; i < nb; ++i)
	{
		if(dst[i]) return __ffs(dst[i]) + (i<<5) + 1;
	}
	return 0;
}
inline int bitmap_ffz(unsigned long *dst, int nb)
{
	int i=0;
	for(; i < nb; ++i)
	{
		if(dst[i]) return __ffz(dst[i]) + (i<<5) + 1;
	}
	return 0;
}
inline int bitmap_find_max_free_region(unsigned long *dst, int nb, int maxcount, int *offset,)//最长的00000
{
	int tmplength = 1;
	int maxlength = 0;
	int ffz = bitmap_ffz(dst, nb);
	if(ffz >= maxcount) return -1;
	*offset = ffz;
	for(; ffz <= maxcount; ++ffz)
	{
		if(!bitmap_find(dst, ffz))
		{
			++tmplength;
		}
		else
		{
			if(tmplength > maxlength)
			{
				*offset = ffz - tmplength;
				maxlength = tmplength;
			}
			tmplength = 1;
		}
	}
	if(tmplength > maxlength)
	{
		*offset = ffz - tmplength;
		maxlength = tmplength;
	}
	return maxlength;
}
inline void bitmap_region_cpl(unsigned long *dst, int offset, int region_size)
{
	int i = 0;
	for(; i < region_size; ++i)
	{
		bitmap_cpl(dst, offset + i);
	}
}
/*just for test!*/
/*
inline int bitmap_foreach(unsigned long *dst, int map_size)
{
	int i;
	for(i = 1; i <= map_size; i++ )
		if(bitmap_find(dst, i)) printf("%d\n", i);
}
*/
#endif //_MINIPWR_BIT_MAP_H

