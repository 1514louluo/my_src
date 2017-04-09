#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "minheap.h"

static int count = 0;
#define ARRAY_SIZE 10


void *test_func(void *arg)
{
        int i=0;
        for(i=0;i<20000;++i){
                __sync_fetch_and_add(&count,1);
        }
        return NULL;
}

void mytest(int *i, ...)
{
	va_list arg_ptr;
        uint32_t j=0;
        uint32_t x=0;
        uint32_t xx=0;
        va_start(arg_ptr, i);
        j=va_arg(arg_ptr, uint32_t);
        x=va_arg(arg_ptr, uint32_t);
        xx=va_arg(arg_ptr, uint32_t);

        va_end(arg_ptr);
	printf("%u, %u, %u\n", j, x, xx);

}

void bits_foreach(void *input, int bytes)
{
	char *bg = (char *)input;
	int i = bytes - 1;
	for(; i >= 0; --i)
	{
		int j = 7;
		for(; j >= 0; --j)	
		{
			printf("%d", !!(bg[i]&(1<<j)));
		}
		printf("\t");
	}
	printf("\n");
}

#include <stdio.h>
#include <signal.h>
char **args;
void exithandle(int sig)
{
       printf("%s : sighup received ",args[1]);
}
int main(int argc,char **argv)
{
	printf("%d\n",sizeof(int*[ARRAY_SIZE]));
  //     args = argv;
 //      signal(SIGHUP,exithandle);
//       pause();
       return 0;
}
/*
int main(int argc, const char *argv[])
{

        pthread_t id[20];
        int i = 0;

        for(i=0;i<20;++i){
                pthread_create(&id[i],NULL,test_func,NULL);
        }

        for(i=0;i<20;++i){
                pthread_join(id[i],NULL);
        }

        printf("%d\n",count);

	mytest(&i);
	mytest(&i, 99);
	mytest(&i, 99, 101);
	mytest(&i, 99, 101, 239328);

	char a[4];
	a[0] = 'a';
	a[1] = 'a';
	a[2] = 'a';
	a[3] = 'a';

	printf("%u\n", strlen(a));

	int i = 100;
	float f = i;
	printf("%f\n", i);
	printf("%d\n", *(int *)&f);

	bits_foreach(&i, 4);
	bits_foreach(&f, 4);

	float d = 100;
	bits_foreach(&d, 4);
	

	min_heap **a = (min_heap **)malloc(sizeof(min_heap *) * 11);
	int i=0;
	for(i = 0; i < 11; ++i)
	{
		a[i] = (min_heap *)malloc(sizeof(min_heap));
	}
	a[0]->priority = 10;
	a[1]->priority = 38;
	a[2]->priority = 30;
	a[3]->priority = 23;
	a[4]->priority = 43;
	a[5]->priority = 4;
	a[6]->priority = 7;
	a[7]->priority = 52;
	a[8]->priority = 43;
	a[9]->priority = 25;
	a[10]->priority = 45;

	for(i = 0; i < 11; ++i)
		printf("%d, ", a[i]->priority);
	printf("\n");

	make_min_heap(a, 11);

	for(i = 0; i < 11; ++i)
		printf("%d, ", a[i]->priority);

	printf("\n");

        return 0;
}
*/
