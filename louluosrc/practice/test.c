#include <stdio.h>
#include <stdlib.h>

#define M 3
#define N 4

int get(int (*a)[N], int i, int j)
{
	return *(a[i]+j);
}

int main()
{
	int a[M][N] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};

	int v;
	
	v = get(a, 2, 1);

	printf("a[2][1] == %d\n", v);

	return 0;
}
