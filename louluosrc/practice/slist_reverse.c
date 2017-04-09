/*	single link list reverse!
 *	author: louluo
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct slist
{
	struct slist *next;
	int  data;
}slist;

typedef struct shead
{
	struct slist *next;
}shead;

static void slist_reverse(shead *head)
{
	if(NULL == head || NULL == head->next)
		return;
	slist *fetch = head->next;

	while(fetch->next)	
	{
		//first del a node from fetch->next
		slist *node = fetch->next;
		fetch->next = node->next;
		
		//second add a node to head
		node->next = head->next;	
		head->next = node;
	}
}

static void slist_insert(shead *head, int data)
{
	if(NULL == head)return;
	slist *new_node = (slist *)malloc(sizeof(slist));
	new_node->data = data;
	new_node->next = head->next;
	head->next = new_node;
}

static void slist_free(shead *head)
{
	if(NULL == head)return;
	slist *fetch = head->next;
	while(fetch)
	{
		slist *tmp = fetch;
		fetch = fetch->next;
		free(tmp);
	}
}

static void slist_foreach(shead *head)
{
	if(NULL == head)return;

	slist *fetch = head->next;
	while(fetch)
	{
		printf("%d-", fetch->data);
		fetch = fetch->next;
	}
	printf("\n");
}

int main()
{
	shead  head = {NULL};
	int i = 0;
	for(; i < 100; ++i)
		slist_insert(&head, i);

	slist_foreach(&head);

	slist_reverse(&head);

	slist_foreach(&head);

	slist_free(&head);
	return 0;

}
