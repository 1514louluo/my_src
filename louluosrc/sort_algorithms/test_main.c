#include "sort.h"

int main()
{
	int A[] = {10,2,4,6,7,3,8,123,5,9};	
//	sort_node *new = sort_node_init(A, 0, 9, sizeof(A)/sizeof(int), INSERT);
//	sort_node *new = sort_node_init(A, 0, 9, sizeof(A)/sizeof(int), BUBBLE);
//	sort_node *new = sort_node_init(A, 0, 9, sizeof(A)/sizeof(int), QUICK);
//	sort_node *new = sort_node_init(A, 0, 9, sizeof(A)/sizeof(int), HEAP);
//	sort_node *new = sort_node_init(A, 0, 9, sizeof(A)/sizeof(int), SELECT);
//	sort_node *new = sort_node_init(A, 0, 9, sizeof(A)/sizeof(int), SHELL);
//	sort_node *new = sort_node_init(A, 0, 9, sizeof(A)/sizeof(int), MERGE);
	sort_node *new = sort_node_init(A, 0, 9, sizeof(A)/sizeof(int), RADIX);
	sort_node_print(new);
	sort_node_run(new);
	sort_node_print(new);
	sort_node_free(new);
	return 0;
}