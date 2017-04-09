#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BITS_PER_BYTE        8
#define BITS_PER_LONG        (BITS_PER_BYTE * sizeof(uint32_t))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define BITS_TO_LONGS(nr)    DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(uint32_t))

#define small_const_nbits(nbits) \
	(__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG)

#define BITMAP_LAST_WORD_MASK(nbits)					\
(									\
	((nbits) % BITS_PER_LONG) ?					\
		(1UL<<((nbits) % BITS_PER_LONG))-1 : ~0UL		\
)

inline void bitmap_zero(uint32_t *dst, size_t nbits) //0初始化
{
	if (small_const_nbits(nbits))
		*dst = 0;
	else {
		int len = BITS_TO_LONGS(nbits) * sizeof(uint32_t);
		memset(dst, 0, len);
	}
}

inline void bitmap_fill(uint32_t *dst, size_t nbits) //1初始化
{
	size_t nlongs = BITS_TO_LONGS(nbits);
	if (!small_const_nbits(nbits)) {
		int len = (nlongs - 1) * sizeof(uint32_t);
		memset(dst, 0xff,  len);
	}
	dst[nlongs - 1] = BITMAP_LAST_WORD_MASK(nbits);
}
inline void bitmap_cpl(uint32_t *dst, size_t nbits) //对某位取反
{
	size_t index = BITS_TO_LONGS(nbits);
	if(!index) return;
	size_t pos = nbits % BITS_PER_LONG;
	if(!pos) pos = BITS_PER_LONG;
	dst[index - 1] ^= 1UL<<(pos-1);
}
inline int bitmap_find(uint32_t *dst, size_t nbits) //查看某位值
{
	size_t index = BITS_TO_LONGS(nbits);
	if(!index) return 0;
	size_t pos = nbits % BITS_PER_LONG;
	if(!pos) pos = BITS_PER_LONG;
	return (dst[index - 1] & (1UL<<(pos-1)));
}
/*just for testing!*/
inline int bitmap_foreach(uint32_t *dst, size_t map_size)
{
	int i;
	for(i = 1; i <= map_size; i++ )
		if(bitmap_find(dst, i)) printf("%d\n", i);
}

int main()
{
	uint32_t map[10];
	bitmap_zero(map, 10*BITS_PER_LONG);
	int i = 1;
	for(; i < 10*BITS_PER_LONG; )
	{
		bitmap_cpl(map, i);
		i += 1;
	}
	bitmap_foreach(map, 10*BITS_PER_LONG);
	return 0;
}
