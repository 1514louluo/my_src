#ifndef _LOCK_FREE_QUEUE_H
#define _LOCK_FREE_QUEUE_H

#define LF_QUEUE_DUMMY_NUM               2

typedef struct _queue_node
{
	struct _queue_node *next;
#ifdef  _DBG
	int  in_queue;
#endif
}lf_queue_node;


typedef struct _queue_node_dummy
{
	lf_queue_node              node;
	int                                in_use;
}lf_queue_node_dummy;

typedef struct  _queue_head
{
	lf_queue_node        		*head;
	lf_queue_node        		*tail;
	lf_queue_node_dummy        lf_dummies[LF_QUEUE_DUMMY_NUM];
	int                                      has_dummy;
}lf_queue_head;


extern void lf_queue_init (lf_queue_head *lf_qhead);

extern void lf_queue_node_init (lf_queue_node *lf_qnode, int to_be_freed) ;

extern void lf_queue_node_free (lf_queue_node *lf_qnode) ;

extern void lf_queue_enqueue (lf_queue_head *lf_qhead, lf_queue_node *lf_qnode) ;

extern lf_queue_node* lf_queue_dequeue (lf_queue_head *lf_qhead) ;


#endif //_LOCK_FREE_QUEUE_H



