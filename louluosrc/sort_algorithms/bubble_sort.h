/*
*   @ bubble sort!
*   @ author: louluo
*/
#ifndef _BUBBLE_H_
#define _BUBBLE_H_

#define SWAP(a, b)                                   \
	do{                                            \
		typeof(*a) tmp = *a;                           \
		*a = *b;    \
		*b = tmp; \
	}while(0)
	

void bubble_sort(int *array, int hindex, int eindex, int array_size)
{
	int i = 0;
	int j = 0;
	for(j = eindex; j >= hindex; --j)
	{
		for(i = hindex; i < j; ++i)
		{
			if(array[i] > array[i+1])	
				SWAP(&array[i], &array[i+1]);
		}
	}
}

#endif /* _BUBBLE_H_*/