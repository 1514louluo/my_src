/*
*	@ quick sort!
*   @ author: louluo
*/

#ifndef _QUICK_H_
#define _QUICK_H_

#define SWAP(a, b)                                   \
	do{                                            \
		typeof(*a) tmp = *a;                           \
		*a = *b;    \
		*b = tmp; \
	}while(0)

static int partition(int *array, int hindex, int eindex)
{
	int mid = hindex;
	int i = hindex;
	for(; i < eindex; ++i)
	{
		if(array[i] < array[eindex])	
		{
			SWAP(&array[i], &array[mid]);
			++mid;
		}
	}
	SWAP(&array[eindex], &array[mid]);
	return mid;
}

void quick_sort(int *array, int hindex, int eindex, int array_size)
{
	if(hindex < eindex)
	{
		int mid = partition(array, hindex, eindex);
		quick_sort(array, hindex, mid - 1, array_size);
		quick_sort(array, mid + 1, eindex, array_size);
	}
}


#endif /*_QUICK_H_*/