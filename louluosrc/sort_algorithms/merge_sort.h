/*
*	@ merge sort!
* 	@ author: louluo
*/
#ifndef _MERGE_H_
#define _MERGE_H_

static void merge_array(int *harray, int *tarray, int harray_size, int tarray_size)
{
	int i = 0, j = 0, k = 0;
	int array[harray_size + tarray_size];

	while(i < harray_size && j < tarray_size)
		array[k++] = (harray[i] < tarray[j])?harray[i++]:tarray[j++];

	while(i < harray_size)
		array[k++] = harray[i++];

	while(j < tarray_size)
		array[k++] = tarray[j++];

	for(i = 0; i < harray_size + tarray_size; ++i)
		harray[i] = array[i];
}

static void __merge_sort(int *array, int array_size)
{
	if(array_size > 1)
	{
		int harray_size = array_size/2;
		int tarray_size = array_size - harray_size;

		int *harray = array;
		int *tarray = array + harray_size;

		__merge_sort(harray, harray_size);
		__merge_sort(tarray, tarray_size);

		merge_array(harray, tarray, harray_size, tarray_size);
	}
}

void merge_sort(int *array, int hindex, int eindex, int array_size)
{
	__merge_sort(array, array_size);
}

#endif /*_MERGE_H_*/