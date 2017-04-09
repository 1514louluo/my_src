/*
 * 	将一个4字节的整数的二进制表示中的001替换为011，输出替换后的整数。
 * 	author: louluo
 */

#include <stdio.h>
typedef unsigned int uint32_t;
typedef unsigned char uchar_t;

void _bm_getmask(int *pos, uchar_t *bmask)
{
        *bmask = (uchar_t)(1 << (*pos % 8));
        *pos /= 8;
}

inline int bm_isset(uchar_t * bm, int pos)
{
        uchar_t   bmask;

        _bm_getmask(&pos, &bmask);
        return !!(bm[pos] & bmask);
}
void bm_foreach(uchar_t *bmap, int size)
{
        int i=size-1;
        for(; i >=0 ; --i)
                printf("%d", bm_isset(bmap, i));
        printf("\n");
}


uint32_t bit_opt(uint32_t input)
{
	int i = 0;
	while(i < 32)
	{
		if((input & (0x7<<i)) == (1<<i))	
		{
			input = input | (3<<i);
			i += 3;
		}
		else ++i;
	}	
	return input;
}


int main()
{
	uint32_t i = 873;
	bm_foreach((uchar_t *)&i, 32);

	uint32_t ret = bit_opt(i);
	bm_foreach((uchar_t *)&ret, 32);

	return 0;
}
