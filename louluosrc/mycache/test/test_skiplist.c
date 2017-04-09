#include "../src/struct/skip_list.h"
#define sampleSize 1000000
int main()
{
        int k=1;

        skip_list_head *head = skip_list_init();

        for(k=0; k<sampleSize; k++)
        {
                skip_list_add(head, k, 1, k);
                skip_list_del(head, k, 1, k);
//		printf("%d\n", __get_random_level());
        }

	sleep(5);
/*
	printf("restart!!\n");
        for(k=0; k<sampleSize; k++)
        {
                skip_list_del(head, k, 1, k);
//		printf("%d\n", __get_random_level());
        }
*/
	skip_list_free(head);
	head = NIL;
	
 
	unsigned long tmp1;
	while(1)
	{
		scanf("%lu", &tmp1);
		skip_list_node *tmp = skip_list_search(head, tmp1, 1, tmp1);
		if(tmp)
		{
			printf("%lu\n", tmp->priority);
			skip_list_del(head, tmp1, 1, tmp1);
		}
		else
			printf("not found!!\n");
	}


		
	return 0;
};


