#include <stdio.h>
#include <stdlib.h>

typedef struct A
{
	int bb;
	int *aa[0];
}A;

void mlloc(int *xx)
{
	xx = (int *)malloc(4);
}

int main()
{
	int input;
	A *ma = (A *)malloc(sizeof(A) + 1000000 * sizeof(int **));	
//	for(input = 0; input < 1000000; ++input)
//		ma->aa[input] = &input;
//	while(1)
	{
//		scanf("%d", &input);	
//		if(1 == input)
		{
			for(input = 0; input < 1000000; ++input)
				free(ma->aa[input]);
//			free(ma);
		}
		sleep(1);
	}

//	A *ma1 = (A *)malloc(sizeof(A));
//	ma1->aa[]
	
	int bb = 5;
	mlloc(&bb1);

	printf("%d\n", bb);
	
	return 0;
}
