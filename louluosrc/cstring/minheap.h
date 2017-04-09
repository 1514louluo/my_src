#ifndef _MIN_HEAP_H
#define _MIN_HEAP_H

#define HPARENT(i) ((i < 1)?0:((i-1)>>1))
#define HLEFT(i)   ((i<<1) + 1)
#define HRIGHT(i)  ((i<<1) + 2)

typedef unsigned long min_heap;

#define SWAP(a, b)               \
	do{                                      \
		typeof(*(a)) tmp = *(a);           \
		*(a) = *(b);                       \
		*(b) = tmp;                        \
	}while(0)
		
void up_heap_node(min_heap **mheap, int index, int length)
{
   int leaf_index = length/2;
   int curindex = index;
   int left=0, right=0;
   
   while(curindex < leaf_index)
   {
   	left  = HLEFT(index);
   	right = HRIGHT(index);
	if(left < length && mheap[left] < mheap[index])
		curindex = left;
	if(right < length && mheap[right] < mheap[curindex])	
		curindex = right;

	if(index != curindex)
	{
		SWAP(&mheap[index], &mheap[curindex]);	
	}
	else break;
	index = curindex;
		
   }
}

void make_min_heap(min_heap **mheap, int length)
{
    int i = length/2 - 1;
    for(; i >= 0; --i)
	up_heap_node(mheap, i, length);
    
}
/*
void min_heap_add()
{
}

void min_heap_add()
{
}
*/
#endif //_MIN_HEAP_H
