/*
*	@ shell sort!
*   @ author: louluo
*/
#ifndef _SHELL_H_
#define _SHELL_H_

void shell_sort(int *array2sort, int hindex, int eindex, int array_size)
{
	int gap = array_size/2;	
	int i, j;
	for(; gap > 0; gap/=2)	
	{
		for(i = gap; i < array_size; ++i)	
		{
			int tmp = array2sort[i];
			for(j = i - gap; j >=0 && array2sort[j] > tmp ; j -= gap)
			{
				array2sort[j+gap]	= array2sort[j];
			}
			array2sort[j+gap] = tmp;
		}
	}
}

#endif /*_SHELL_H_*/
