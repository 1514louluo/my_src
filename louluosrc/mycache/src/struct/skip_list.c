#include "skip_list.h"

static skip_list_head *__skip_list_init(void)
{
        skip_list_head *skip_list = (skip_list_head *)P_MALLOC(SKIP_LIST_HEAD_SIZE);
        if(NIL == skip_list)
                return NIL;

        skip_list->level = 0;
        skip_list->head = NEW_SKIP_LIST_NODE(MAX_SKIP_LIST_LEVEL);

        if(NIL == skip_list->head)
        {
                P_FREE(skip_list);
                return NIL;
        }
        int i = 0;
        for(; i < MAX_SKIP_LIST_LEVEL; ++i)
        {
                skip_list->head->elem[i] = NIL;
        }
        skip_list->head->nlevel = MAX_SKIP_LIST_LEVEL;
        skip_list->head->priority = MAX_PRIORITY;

        return skip_list;
}

static inline int __random(void)
{
        struct timeval tv;
        gettimeofday(&tv, NIL);
        srand(tv.tv_usec + tv.tv_sec * 1000000);

        return (int)rand();
}
static int  __get_random_level(void)
{
        register int level = 0;
        register int b;
        static int randomBits;
        randomBits =  __random();
        static int randomsLeft = MAX_SKIP_LIST_LEVEL_INDEX;
        do
        {
                b = randomBits&3;
                if(!b) level++;
                randomBits>>=2;
                if(--randomsLeft == 0)
                {
                        randomBits = __random();
                        randomsLeft = MAX_SKIP_LIST_LEVEL_INDEX;
                }
        }
        while(!b);
        return(level>MAX_SKIP_LIST_LEVEL_INDEX ? MAX_SKIP_LIST_LEVEL_INDEX : level);
}

skip_list_head *skip_list_init(void)
{
        return __skip_list_init();
}

skip_list_node * skip_list_add(skip_list_head *shead, skip_list_node *snode, ulong_t priority, ulong_t ack_hash_1, ulong_t ack_hash_2)
{
        if(NIL == shead)
                return NIL;

        int slevel = shead->level;
        skip_list_node *update[MAX_SKIP_LIST_LEVEL];
        skip_list_node *phead = shead->head;
        skip_list_node *pnode = NIL;

        do
        {
                while((pnode = phead->elem[slevel]) && pnode->priority <= priority)
                        phead = pnode;

                update[slevel] = phead;
        }
        while(--slevel >= 0);	

        while(pnode && priority == pnode->priority)
        {
                if(ack_hash_1 == pnode->ack.ack_hash_1 && ack_hash_2 ==  pnode->ack.ack_hash_2)
                {
                        return pnode;
                }
                pnode = pnode->elem[0];
        }

        slevel = __get_random_level();

        if(slevel > shead->level)
        {
                slevel = ++shead->level;
                update[slevel] = shead->head;
        }
		
	 snode = NEW_SKIP_LIST_NODE(slevel + 1);
        snode->priority = priority;
        snode->ack.ack_hash_1 = ack_hash_1;
        snode->ack.ack_hash_2 = ack_hash_2;

        do
        {
                phead = update[slevel];
                snode->elem[slevel] = phead->elem[slevel];
                phead->elem[slevel] = snode;
        }
        while(--slevel >= 0);
        return snode;
}

void skip_list_free(skip_list_head *shead)
{
	 if(NIL == shead) return;
        skip_list_node  *p = shead->head;
        skip_list_node *q;
        do
        {
                q = p->elem[0];
		  P_FREE(p);
                p = q;
        }
        while(p!=NIL);
        P_FREE(shead);
}

skip_list_node * skip_list_del(skip_list_head *shead, ulong_t priority, ulong_t ack_hash_1, ulong_t ack_hash_2)
{
        if(NIL == shead)
                return NIL;

        int slevel = shead->level;
        int tmplevel = shead->level;
        skip_list_node *update[MAX_SKIP_LIST_LEVEL];
        skip_list_node *phead = shead->head;
        skip_list_node *pnode = NIL;
		
        do
        {
                while((pnode = phead->elem[slevel]) && pnode->priority < priority)
                        phead = pnode;

                update[slevel] = phead;
        }
        while(--slevel >= 0);	

        while(pnode && priority == pnode->priority)
        {
                if(ack_hash_1 == pnode->ack.ack_hash_1 && ack_hash_2 ==  pnode->ack.ack_hash_2)
                {
                        for(slevel=0; slevel<=tmplevel && (phead=update[slevel])->elem[slevel] == pnode; slevel++)
                                phead->elem[slevel] = pnode->elem[slevel];

			   P_FREE(pnode);
                        while(shead->head->elem[tmplevel] == NIL && tmplevel > 0)
                                tmplevel--;
                        shead->level = tmplevel;
                        return pnode;
                }
                pnode = pnode->elem[0];
        }

        return NIL;
}

skip_list_node  *skip_list_search(skip_list_head *shead, ulong_t priority, ulong_t ack_hash_1, ulong_t ack_hash_2)
{
        if(NIL == shead)
                return NULL;

        int slevel = shead->level;
        skip_list_node *phead = shead->head;
        skip_list_node *pnode = NIL;

        do
        {
                while((pnode = phead->elem[slevel]) && pnode->priority < priority)
                        phead = pnode;
        }
        while(--slevel >= 0);	

        while(pnode && priority == pnode->priority)
        {
                if(ack_hash_1 == pnode->ack.ack_hash_1 && ack_hash_2 ==  pnode->ack.ack_hash_2)
                {
                        return pnode;
                }
                pnode = pnode->elem[0];
        }

        return NULL;
}


