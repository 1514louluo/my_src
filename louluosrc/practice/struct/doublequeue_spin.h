/*
*	A simple double queue implementation. <double ring linklist implement the queue, just one producer and one consumer>
*	License: MIT
* 	Copyright (C) 2014 louluo <18915413902@163.com> if there is some bug, please let me know!
*/
#ifndef _DOUBLE_QUEUE_H
#define _DOUBLE_QUEUE_H

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>

#include <stdlib.h>
//#include "jemalloc"
#define P_MALLOC malloc      /* here can use jemalloc, improve performance 1/3 */
#define P_FREE   free        /* here can use jemalloc, improve performance 1/3 */

#define COND_TIMEOUT 3       /* semp try wait cond timeout */

/* encapsulate for one function only one return */
#define	return_val_if(condition, return_value) 							\
do{											 	\
  if((condition))										\
    {												\
      return (return_value);									\
    }												\
}while(0)

#define	return_val_if_fail(condition, return_value) 						\
do{											 	\
  if(!(condition))										\
    {												\
      return (return_value);									\
    }												\
}while(0)

#define	return_if(condition) 									\
do{											 	\
  if((condition))										\
    {												\
      return;											\
    }												\
}while(0)


#define	return_if_fail(condition) 								\
do{											 	\
  if(!(condition))										\
    {												\
      return;											\
    }												\
}while(0)

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*pfree_handler)(void *);

struct double_queue_node
{
	struct  double_queue_node           *next;        /* double linklist node */
	struct  double_queue_node           *prev;        /* double linklist node */
	void                                *data;        /* data, elem is a pointer */
};

struct double_queue_head
{
	struct double_queue_node            *in;          /* producer */
	struct double_queue_node            *out;         /* consumer */
	pthread_spinlock_t                  *lock;        /* swap in and out lock */
	sem_t                               *no_empty;    /* producer is empty? */
};

extern struct double_queue_head *double_queue_init(pthread_spinlock_t *lock, sem_t *cond);
extern void double_queue_free(struct double_queue_head *head, pfree_handler handler);
extern int double_queue_put(struct double_queue_head *head, void *data);
extern void *double_queue_get(struct double_queue_head *head);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif /*_DOUBLE_QUEUE_H*/
