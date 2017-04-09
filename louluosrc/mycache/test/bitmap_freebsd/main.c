#include "fb_bitmap.h"
#include <stdio.h>
int main()
{

   uchar_t bmap[2];
   bm_zero(bmap, 10);
   bm_setbit(bmap, 0);
//   bm_setbit(bmap, 1);
   bm_setbit(bmap, 2);
   bm_setbit(bmap, 4);
   bm_setbit(bmap, 7);
 //  bm_setbit(bmap, 11);
//   bm_setbit(bmap, 15);

   bm_foreach(bmap, 2*8);

   printf("bmap[0] = %d; bmap[1] = %d\n", bmap[0], bmap[1]);

   printf("1<==>%d\n", bm_ffs(bmap, 2));
   printf("0<==>%d\n", bm_ffz(bmap, 2));

//   printf("0<==>%d\n", _bm_ffz(4));
//   printf("0<==>%d\n", _bm_ffs(241));
/*
   int pos;
    int size = bm_max_unset_region(bmap, 2, &pos);
   printf("max_pos = %d, max_size = %d\n", pos, size);
   
   bm_setbit_region(bmap, pos, size);

   bm_foreach(bmap, 2*8);


   size = bm_max_unset_region(bmap, 2, &pos);
   printf("max_pos = %d, max_size = %d\n", pos, size);

   size = bm_max_set_region(bmap, 2, &pos);
   printf("max_pos = %d, max_size = %d\n", pos, size);
*/
   int size = bm_lastset(bmap, 16);
   printf("lastset = %d\n", size);
   return 0;
}
