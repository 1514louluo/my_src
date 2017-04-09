/*
*	@ select sort! 
*	@ author: louluo
*/
#ifndef _SELECT_H_
#define _SELECT_H_

#define SWAP(a, b)                                   \
	do{                                            \
		typeof(*a) tmp = *a;                           \
		*a = *b;    \
		*b = tmp; \
	}while(0)



void select_sort(int *array, int hindex, int eindex, int array_size)
{
	int i, j, min;
	for(j = hindex; j <= eindex; ++j)
	{
		min = j;
		for(i = j; i <= eindex; ++i)
		{
			if(array[i] < array[min])	
					min= i;
		}
		if(min != j)
			SWAP(&array[min], &array[j]);
	}
}

#endif /*_SELECT_H_*/