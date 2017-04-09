/*
*	@ this is a sort virtual interface!
*	@ author louluo
*/
#ifndef _SORT_H_
#define _SORT_H_
#include "insert_sort.h"
#include "bubble_sort.h"
#include "quick_sort.h"
#include "heap_sort.h"
#include "select_sort.h"
#include "shell_sort.h"
#include "merge_sort.h"
#include "radix_sort.h"
#include <stdlib.h>
#include <stdio.h>

enum {  
	BITMAP=1,
	BUBBLE=2,
	BUCKET=3,
	COUNT=4,
	INSERT=5,
	MERGE=6,
	HEAP=7,
	QUICK=8,
	RADIX=9,
	SELECT=10,
	SHELL=11,
};

typedef void(*sort_cb)(int *, int, int, int);

struct  sort_node
{
	int *array2sort;
	int type;
	int hindex;
	int eindex;
	int array_len;
	sort_cb cb;
};
typedef struct sort_node sort_node;

sort_node *sort_node_init(int *array, int hindex, int eindex, int array_len, int type)
{
	sort_node *new = (sort_node *)malloc(sizeof(sort_node));
	if(NULL == new) return NULL;
	new->array2sort = array;
	new->hindex = hindex;
	new->eindex = eindex;
	new->array_len = array_len;
	new->type = type;

	switch(type)
	{
		case INSERT: 
			new->cb=insert_sort;
			break;
		case BUBBLE:
			new->cb=bubble_sort;
			break;
		case QUICK:
			new->cb=quick_sort;
			break;
		case HEAP:
			new->cb=heap_sort;
			break;
		case SELECT:
			new->cb=select_sort;
			break;
		case SHELL:
			new->cb=shell_sort;
			break;
		case MERGE:
			new->cb=merge_sort;
			break;
		case RADIX:
			new->cb=radix_sort;
			break;
		default:
			new->type = INSERT;
			new->cb  = insert_sort;
			break;
	}
	return new;
}

void sort_node_run(sort_node *snode)
{
	if(NULL == snode) return;
	return snode->cb(snode->array2sort, snode->hindex, snode->eindex, snode->array_len);
}

static void raw_print(int *A, int hindex, int eindex)
{
	int i =hindex;
	for(; i <= eindex; ++i)
	{
		printf("%d\t", A[i]);
	}
	printf("\n");
}

void sort_node_print(sort_node *snode)
{
	if(NULL == snode) return;
	raw_print(snode->array2sort, snode->hindex, snode->eindex);
}

void sort_node_free(sort_node *snode)
{
	if(NULL == snode) return;
	free(snode);
}

#endif /*_SORT_H_*/