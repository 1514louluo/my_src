/*
*	@ radix sort!
*	@ author: louluo
*/

#ifndef _RADIX_H_
#define _RADIX_H_


void radix_sort(int *array, int hindex, int eindex, int array_size)
{
// We must know how many digits of array elem in array! here we define 3
#define GET_POS_NB(srcnb, d, ary)  ({int i=0;int nb=(srcnb);for(; i < (d); ++i)nb /=(ary); nb%=(ary);nb;})
#define DEPTH 3
	int d = 0;
	for(; d < DEPTH; ++d)
	{
		//here need a stable sort! we use insert sort!
		int tmp = 0;
		int i = hindex + 1;
		int j = hindex;
		for(; i <= eindex; ++i)	
		{
			tmp = array[i];
			j = i - 1;
//			while((hindex <= j) && (tmp < array[j]))
			while((hindex <= j) && GET_POS_NB(tmp, d, 10) < GET_POS_NB(array[j],d,10))
			{
				array[j+1]	= array[j];
				--j;
			}
			array[j+1] = tmp;
		}
	}
}

#endif /*_RADIX_H_*/