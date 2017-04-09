#include "lock_free_queue.h"
#include <assert.h>

#define INVALID_NEXT	((void*)-1)
#define END_MARKER	((void*)-2)
#define FREE_NEXT	((void*)-3)

void lf_queue_init(lf_queue_head *lf_qhead)
{
	int i;
	for (i = 0; i < LF_QUEUE_DUMMY_NUM; ++i) {
		lf_qhead->lf_dummies[i].node.next = (i == 0) ? END_MARKER : FREE_NEXT;
		lf_qhead->lf_dummies[i].in_use = i == 0 ? 1 : 0;
#ifdef _DBG
		lf_qhead->lf_dummies[i].node.in_queue = i == 0 ? 1 : 0;
#endif
	}

	lf_qhead->head = lf_qhead->tail = &lf_qhead->lf_dummies [0].node;
	lf_qhead->has_dummy = 1;
}

void lf_queue_node_init(lf_queue_node *lf_qnode, int to_be_freed) 
{
	lf_qnode->next = to_be_freed ? INVALID_NEXT : FREE_NEXT;
#ifdef _DBG
	lf_qnode->in_queue = FALSE;
#endif
}

void lf_queue_node_free(lf_queue_node *lf_qnode) 
{
	assert (lf_qnode->next == INVALID_NEXT);
#ifdef _DBG
	assert (!lf_qnode->in_queue);
#endif
	lf_qnode->next = FREE_NEXT;
}

void lf_queue_enqueue(lf_queue_head *lf_qhead, lf_queue_node *lf_qnode) ;

lf_queue_node* lf_queue_dequeue(lf_queue_head *lf_qhead) ;



