/*
 *	binary tree! no delete opt!
 *	author: louluo
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct btree
{
	struct btree *left;
	struct btree *right;
	int           data; // simple anyway!
};
typedef struct btree btree;

#define NEW_NODE(i) ({btree *tmp = (btree *)malloc(sizeof(btree)); tmp->left=tmp->right=NULL; tmp->data = i; tmp;})

static void btree_insert(btree *head, int data)
{
	if(NULL == head) 
	{
//		head = NEW_NODE(data);
		return;
	}
	if(-1 == head->data)
	{
		head->data = data;
		return;
	}
	while(1)
	{
		if(data <= head->data)
		{
			if(NULL == head->left)
			{
				head->left = NEW_NODE(data);
				break;
			}
			head = head->left;	
		}
		else
		{
			if(NULL == head->right)
			{
				head->right = NEW_NODE(data);
				break;
			}
			head = head->right;
		}
	}

}

static void btree_free(btree *head)
{
	if(head)
	{
		btree_free(head->left);
		btree_free(head->right);
		free(head);	
	}
}

static void btree_foreach_preorder(btree *head)
{
	if(head)
	{
		btree_foreach_preorder(head->left);
		printf("%d\t", head->data);
		btree_foreach_preorder(head->right);
	}
}

static void btree_foreach_midorder(btree *head)
{
	if(head)
	{
		printf("%d\t", head->data);
		btree_foreach_midorder(head->left);
		btree_foreach_midorder(head->right);
	}
}

static void btree_foreach_postorder(btree *head)
{
	if(head)
	{
		btree_foreach_postorder(head->left);
		btree_foreach_postorder(head->right);
		printf("%d\t", head->data);
	}
}

int main()
{
	int i = 0;
	btree *head = NEW_NODE(-1);
	srand((unsigned)time(0));
	for(; i < 10; ++i)
	{
		int data = (int)rand()%10;
		printf("%d\t", data);
		btree_insert(head,data);
	}
	printf("\n");
	printf("preorder:\t");
	btree_foreach_preorder(head);
	printf("\n");
	printf("midorder:\t");
	btree_foreach_midorder(head);
	printf("\n");
	printf("postorder:\t");
	btree_foreach_postorder(head);
	printf("\n");

	btree_free(head);
	return 0;
}

