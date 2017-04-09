#ifndef _BITMAP_H_
#define _BITMAP_H_
#include "cdef.h"

typedef unsigned char uchar_t;

#define BITS_PER_BYTE            8

#define BYTES_PER_UCHAR          1 
#define POWER_PER_UNCHAR_BYTES    0      /* 2 pow 0 = 1*/ 

#define BITS_PER_UCHAR         (BITS_PER_BYTE * BYTES_PER_UCHAR)
#define POWER_PER_UNCHAR_BITS    3      /* 2 pow 3 = 8*/ 

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define BITS_TO_UCHARS(nr)    DIV_ROUND_UP(nr, BITS_PER_UCHAR)




extern void _bm_getmask(int *pos, uchar_t *bmask);

extern int _bm_ffs(uchar_t x);  

extern int _bm_ffz(uchar_t x);  

extern void bm_zero(uchar_t *bm, int nb); //0初始化

extern void bm_fill(uchar_t *bm, int nb); //1初始化

extern void bm_setbit(uchar_t *bm, int pos);

extern void bm_clrbit(uchar_t * bm, int pos);

extern int bm_isset(uchar_t * bm, int pos);

extern int bm_isset_region(uchar_t *bm, int pos, int range);

extern int bm_ffs(uchar_t *bm, int nb); //查找第一位为1的位置

extern int bm_ffz(uchar_t *bm, int nb); //查找第一位为0的位置

extern int bm_max_unset_region(uchar_t *bm, int nb, int *pos);

extern int bm_max_set_region(uchar_t *bm, int nb, int *pos);

extern void bm_setbit_region(uchar_t *bm, int pos, int region_size);

extern void bm_clrbit_region(uchar_t *bm, int pos, int region_size);
   
extern int bm_lastset(uchar_t * bm, int size);

/* for test */
extern void bm_foreach(uchar_t *bmap, int size);


#endif/* !_BITMAP_H */
