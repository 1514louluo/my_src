/*
*	@ insert sort！
*	@ author: louluo
*/

#ifndef _INSERT_H_
#define _INSERT_H_

void insert_sort(int *array2sort, int hindex, int eindex, int array_size)
{
	int tmp = 0;
	int i = hindex + 1;
	int j = hindex;
	for(; i <= eindex; ++i)	
	{
		tmp = array2sort[i];
		j = i - 1;
		while((hindex <= j) && (tmp < array2sort[j]))
		{
			array2sort[j+1]	= array2sort[j];
			--j;
		}
		array2sort[j+1] = tmp;
	}
}

#endif /*_INSERT_H_*/