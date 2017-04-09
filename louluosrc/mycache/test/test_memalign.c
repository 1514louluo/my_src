#include <stdio.h>
#include <stdlib.h>
int main()
{
		struct A
		{
				char x;
				int m;
		};
		printf("%d\n", sizeof(struct A));
		struct A *a = (struct A *)memalign(2, 8);
		printf("%d\n", sizeof(*a));
		return 0;
}
