#include <stdio.h>
#include <stdlib.h>

struct readbuf
{
	struct readbuf *next;
	char block[1024];
	int    capacity;
};

struct readbuf_head
{
	struct readbuf *next;
	struct readbuf *tail;
};

#define NEW_READBUF_NODE(i) ({struct readbuf *node = (struct readbuf *)malloc(sizeof(struct readbuf)); node->next=NULL;node->capacity=i;node;})
#define NEW_READBUF_HEAD ({struct readbuf_head *head = (struct readbuf_head *)malloc(sizeof(struct readbuf_head)); head->next=head->tail=NEW_READBUF_NODE(-1); head;})

#define READBUF_APPEND(head, i) do{struct readbuf *newnode = NEW_READBUF_NODE(i); (head)->tail->next=newnode; (head)->tail = newnode;}while(0);
#define READBUF_FREE(head) 	    do{while((head)){struct readbuf *node = (head);(head)=(head)->next;free(node);}}while(0);

#define READBUF_FOREACH(head) 	    do{struct readbuf *node = (head)->next;while((node)){printf("%d\t", node->capacity);node=node->next;}}while(0);

int main()
{
	int i = 0;
	struct readbuf_head *head = NEW_READBUF_HEAD;
	for(; i < 20; ++i)
	{
		READBUF_APPEND(head, i);
	}

	READBUF_FOREACH(head);

	READBUF_FREE(head);
	return 0;
}
