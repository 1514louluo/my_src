/*
*   @ heap sort!
*   @ author: louluo
*/

#ifndef _HEAP_H_
#define _HEAP_H_

#define LEFT(x)               ((x<<1)+1)
#define RIGHT(x)              ((x<<1)+2)
#define SWAP(a, b)                                   \
	do{                                            \
		typeof(*a) tmp = *a;                           \
		*a = *b;    \
		*b = tmp; \
	}while(0)

static void up_heap(int *array, int index, int array_size)
{
	int leaf_index = array_size/2;
	int current = index;
	int lindex=0, rindex=0;

	while(current < leaf_index)
	{
		lindex = LEFT(index);
		rindex = RIGHT(index);
		if(lindex < array_size && array[lindex] > array[current])
			current = lindex;
		if(rindex < array_size && array[rindex] > array[current])
			current = rindex;

		if(current != index)
			SWAP(&array[current], &array[index]);
		else break;
		index = current;
	}
}

static void max_heap(int *array, int hindex, int eindex, int array_size)
{
	int i = array_size/2 - 1;
	for(; i >= hindex; --i)
	{
		up_heap(array, i, array_size);
	}
}

void heap_sort(int *array, int hindex, int eindex, int array_size)
{
	int tail_index = eindex;
	max_heap(array, hindex, eindex, array_size);
	while(array_size > 0 && tail_index > hindex)
	{
		SWAP(&array[tail_index], &array[hindex]);
		--tail_index;
		up_heap(array, hindex, --array_size);
	}
}

#endif /*_HEAP_H*/