/*
 *	 题目：输入一颗二元树，从上往下按层打印树的每个结点，同一层中按照从左往右的顺序打印。
 *
 *	层次遍历
 *
 * 	author: louluo
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct btree
{
	struct btree *left;
	struct btree *right;
	int           data;
}btree;

static void btree_level_foreach()
{
}

static btree_insert(btree *head, int data)
{
}

static inline btree_init(void)
{
	btree *head = (btree *)malloc(sizeof(btree));
	if(head)
	{
		head->left = head->right = NULL;
		head->data = ;
	}
}

static inline btree_free(btree *head)
{
}
