#ifndef _FB_BITMAP_H_
#define _FB_BITMAP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uchar_t;

#define BITS_PER_BYTE            8

#define BYTES_PER_UCHAR          1 
#define POWER_PER_UNCHAR_BYTES    0      /* 2 pow 0 = 1*/ 

#define BITS_PER_UCHAR         (BITS_PER_BYTE * BYTES_PER_UCHAR)
#define POWER_PER_UNCHAR_BITS    3      /* 2 pow 3 = 8*/ 

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define BITS_TO_UCHARS(nr)    DIV_ROUND_UP(nr, BITS_PER_UCHAR)




static void _bm_getmask(int *pos, uchar_t *bmask)
{
	*bmask = (uchar_t) (1 << (*pos % BITS_PER_UCHAR));
	*pos /= BITS_PER_UCHAR;
}

static inline int _bm_ffs(uchar_t x)  
{
	int r = 0;

	if (!x)
		return 0;
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

static inline int _bm_ffz(uchar_t x)  
{
	return _bm_ffs(~x & 0x0ff);
}

inline void bm_zero(uchar_t *bm, int nb) //0初始化
{
	memset(bm, 0, nb);
}

inline void bm_fill(uchar_t *bm, int nb) //1初始化
{
	memset(bm, 0xff,  nb);
}

inline void bm_setbit(uchar_t *bm, int pos)
{
	uchar_t   bmask;

	_bm_getmask(&pos, &bmask);
	bm[pos] |= bmask;
}

inline void bm_clrbit(uchar_t * bm, int pos)
{
	uchar_t   bmask;

	_bm_getmask(&pos, &bmask);
	bm[pos] &= ~bmask;
}

inline int bm_isset(uchar_t * bm, int pos)
{
	uchar_t   bmask;

	_bm_getmask(&pos, &bmask);
	return !!(bm[pos] & bmask);
}

inline int bm_ffs(uchar_t *bm, int nb) //查找第一位为1的位置
{
	int i=0;
	for(; i < nb; ++i)
	{
		if(bm[i]) return _bm_ffs(bm[i]) + (i<<POWER_PER_UNCHAR_BITS);
	}
	return -1;
}

inline int bm_ffz(uchar_t *bm, int nb) //查找第一位为0的位置
{
	int i=0;
	for(; i < nb; ++i)
	{
		if(0xff > bm[i]) return _bm_ffz(bm[i]) + (i<<POWER_PER_UNCHAR_BITS);
	}
	return -1;
}

inline int bm_max_unset_region(uchar_t *bm, int nb, int *pos)
{
	int tmplength = 0;
	int maxlength = 0;
	int ffz = bm_ffz(bm, nb);
	if(ffz < 0) return -1;
	*pos = ffz++;
	nb = nb<<POWER_PER_UNCHAR_BITS;
	for(; ffz < nb; ++ffz)
	{
		if(!bm_isset(bm, ffz))
		{
			++tmplength;
		}
		else
		{
			if(tmplength > maxlength)
			{
				*pos = ffz - tmplength;
				maxlength = tmplength;
			}
			tmplength = 0;
		}
	}
	if(tmplength > maxlength)
	{
		*pos = ffz - tmplength;
		maxlength = tmplength;
	}
	return maxlength;
}

inline int bm_max_set_region(uchar_t *bm, int nb, int *pos)
{
	int tmplength = 0;
	int maxlength = 0;
	int ffs = bm_ffs(bm, nb);
	if(ffs < 0) return -1;
	*pos = ffs++;
	nb = nb<<POWER_PER_UNCHAR_BITS;
	for(; ffs < nb; ++ffs)
	{
		if(bm_isset(bm, ffs))
		{
			++tmplength;
		}
		else
		{
			if(tmplength > maxlength)
			{
				*pos = ffs - tmplength;
				maxlength = tmplength;
			}
			tmplength = 0;
		}
	}
	if(tmplength > maxlength)
	{
		*pos = ffs - tmplength;
		maxlength = tmplength;
	}
	return maxlength;
}

inline void bm_setbit_region(uchar_t *bm, int pos, int region_size)
{
	int i=0;
	for(; i < region_size; ++i)
	{
		bm_setbit(bm, pos + i);
	}
}

inline void bm_clrbit_region(uchar_t *bm, int pos, int region_size)
{
	int i=0;
	for(; i < region_size; ++i)
	{
		bm_clrbit(bm, pos + i);
	}
}

inline int bm_lastset(uchar_t * bm, int size)
{
	int             szmap = (size / BITS_PER_UCHAR) + !!(size % BITS_PER_UCHAR);
	int             at = 0;
	int             pos = 0;
	int             ofs = 0;

	while (pos < szmap) {
		unsigned char   bmv = bm[pos++];
		unsigned char   bmask = 1;

		while (bmask & 0xff) {
			if ((bmv & bmask) != 0)
				ofs = at;
			bmask <<= 1;
			++at;
		}
	}
	return ofs;
}


/* for test */
void bm_foreach(uchar_t *bmap, int size)
{
	int i=0;
	for(;i < size; ++i)
		printf("%d", bm_isset(bmap, i));
	printf("\n");
}


#endif/* !_BITMAP_H */
