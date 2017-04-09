#ifndef _SKIP_LIST_H
#define _SKIP_LIST_H
#include "cdef.h"
//#include "kvcache.h"

typedef struct skip_list_ack
{
        ulong_t 		   ack_hash_1;
        ulong_t 		   ack_hash_2;
} skip_list_ack;

struct skip_list_node
{
	 void                 *phead;
        ulong_t  		   priority;
	 ulong_t             nlevel;
        skip_list_ack     ack;
        struct skip_list_node   *elem[0];
}  __attribute__((aligned(sizeof(long_t))));

typedef struct skip_list_node skip_list_node;

#define T_H skip_list_head
typedef struct skip_list_head
{
        int  				  level;
        skip_list_node 	*head;
} skip_list_head;
#undef T_H

/*size*/
#define  MAX_PRIORITY   ULONG_MAX

#define SKIP_LIST_HEAD_SIZE sizeof(skip_list_head)
#define SKIP_LIST_HEAD_PSIZE sizeof(skip_list_head *)

#define SKIP_LIST_NODE_SIZE sizeof(skip_list_node)
#define SKIP_LIST_NODE_PSIZE sizeof(skip_list_node *)

/*mem*/
#define NIL NULL
#define NEW_SKIP_LIST_NODE(level)       (skip_list_node *)P_MALLOC(SKIP_LIST_NODE_SIZE + (level) * SKIP_LIST_NODE_PSIZE)


/*level*/
#define MAX_SKIP_LIST_LEVEL                                            16
#define MAX_SKIP_LIST_LEVEL_INDEX                                (MAX_SKIP_LIST_LEVEL-1)


#ifdef __cplusplus
extern "C" {
#endif

skip_list_head  *skip_list_init(void);
void skip_list_free(skip_list_head *shead);

skip_list_node  *skip_list_add(skip_list_head *shead, skip_list_node *snode, ulong_t priority, ulong_t ack_hash_1, ulong_t ack_hash_2);
skip_list_node  *skip_list_del(skip_list_head *shead, ulong_t priority, ulong_t ack_hash_1, ulong_t ack_hash_2);
skip_list_node  *skip_list_search(skip_list_head *shead, ulong_t priority, ulong_t ack_hash_1, ulong_t ack_hash_2);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_SKIP_LIST_H
