#include "kvcache.h"

kvcache_head kvcache_head_ring[MAX_AREA_SIZE];

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
        static int randomsLeft = MAX_SK_LEVEL_INDEX;
        do
        {
                b = randomBits&3;
                if(!b) level++;
                randomBits>>=2;
                if(--randomsLeft == 0)
                {
                        randomBits = __random();
                        randomsLeft = MAX_SK_LEVEL_INDEX;
                }
        }
        while(!b);
        return(level>MAX_SK_LEVEL_INDEX ? MAX_SK_LEVEL_INDEX : level);
}

static void __kv_node_free(kvcache_node *kv_node)
{
	if(kv_node->key) P_FREE(kv_node->key);
	if(kv_node->value) P_FREE(kv_node->value);
	if(kv_node) P_FREE(kv_node);
}

static void __kvcache_free(kvcache_head *kv_head)
{
	 if(NIL == kv_head) return;
        kvcache_node  *p = kv_head->head;
        kvcache_node  *q;
        do
        {
                q = p->elem[0];
		  __kv_node_free(p);
                p = q;
        }
        while(p!=NIL);
}

kvcache_node *__kvcache_set(kvcache_head *shead, ulong_t priority, ulong_t ack_hash_1, ulong_t ack_hash_2)
{
        if(NIL == shead)
                return NIL;

        int slevel = shead->level;
        kvcache_node *update[MAX_SK_LEVEL];
        kvcache_node *phead = shead->head;
        kvcache_node *pnode = NIL;

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
		
	 pnode = NEW_KVCACHE_NODE_NODE(slevel + 1);
	 pnode->key = NIL;
	 pnode->value = NIL;
	 pnode->klen = 0;
	 pnode->vlen = 0;
        pnode->priority = priority;
        pnode->ack.ack_hash_1 = ack_hash_1;
        pnode->ack.ack_hash_2 = ack_hash_2;

        do
        {
                phead = update[slevel];
                pnode->elem[slevel] = phead->elem[slevel];
                phead->elem[slevel] = pnode;
        }
        while(--slevel >= 0);
        return pnode;
}

int  __kvcache_del(kvcache_head *shead, ulong_t priority, ulong_t ack_hash_1, ulong_t ack_hash_2)
{
        if(NIL == shead)
                return NIL;

        int slevel = shead->level;
        int tmplevel = shead->level;
        kvcache_node *update[MAX_SK_LEVEL];
        kvcache_node *phead = shead->head;
        kvcache_node *pnode = NIL;
		
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

			   __kv_node_free(pnode);
                        while(shead->head->elem[tmplevel] == NIL && tmplevel > 0)
                                tmplevel--;
                        shead->level = tmplevel;
                        return 0;
                }
                pnode = pnode->elem[0];
        }

        return -1;
}

kvcache_node  *__kvcache_get(kvcache_head *shead, ulong_t priority, ulong_t ack_hash_1, ulong_t ack_hash_2)
{
        if(NIL == shead)
                return NULL;

        int slevel = shead->level;
        kvcache_node *phead = shead->head;
        kvcache_node *pnode = NIL;

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


int kvcache_init(void)
{
	int i=0, j=0;
	for(; i < MAX_AREA_SIZE; ++i)
	{
		kvcache_head_ring[i] = (kvcache_head){0, NEW_KVCACHE_NODE_NODE(MAX_SK_LEVEL)};
		if(NIL == kvcache_head_ring[i].head) return -1;

		for(j = 0; j < MAX_SK_LEVEL; ++j)
		{
			kvcache_head_ring[i].head->elem[j] = NIL;
		}
		kvcache_head_ring[i].head->priority = MAX_PRIORITY;
	}
	return 0;
}

void kvcache_free(void)
{
	int i=0;
	for(; i < MAX_AREA_SIZE; ++i)
	{
		__kvcache_free(&kvcache_head_ring[i]);
	}
}

int kvcache_opt_proxy(char cmd)
{
	
}

int kvcache_set(const char *key, const char *value,  ulong_t klen, ulong_t vlen, long_t ttl)
{
	ulong_t  ack_hash_0 = hash_string(key, klen, 0);
	ulong_t  ack_hash_1 = hash_string(key, klen, 1);
	ulong_t  ack_hash_2 = hash_string(key, klen, 2);

	ulong_t priority = ack_hash_0/MAX_AREA_SIZE;

	kvcache_node *kv_node = __kvcache_set(&kvcache_head_ring[ack_hash_0%MAX_AREA_SIZE],  priority, ack_hash_1, ack_hash_2);
	
	if(NULL == kv_node) return -1;

	/*old kv_node*/
	if(kv_node->key && kv_node->value)
	{
		P_FREE(kv_node->value);
		kv_node->value = (char *)P_MALLOC(vlen);
		memcpy(kv_node->value, value, vlen);
		kv_node->vlen = vlen;
		return 0;	
	}

	/*new kv_node*/
	kv_node->key = (char *)P_MALLOC(klen);
	memcpy(kv_node->key, key, klen);
	kv_node->value = (char *)P_MALLOC(vlen);
	memcpy(kv_node->value, value, vlen);

	kv_node->klen=klen;
	kv_node->vlen=vlen;
//	kv_node->type = 0;
	kv_node->ttl = ttl;
	kv_node->tms = (long_t)time((time_t *)NULL);

	return 0;
}

char *kvcache_get(const char *key, ulong_t klen)
{
	ulong_t  ack_hash_0 = hash_string(key, klen, 0);
	ulong_t  ack_hash_1 = hash_string(key, klen, 1);
	ulong_t  ack_hash_2 = hash_string(key, klen, 2);

	ulong_t priority = ack_hash_0/MAX_AREA_SIZE;

	kvcache_node *kv_node = __kvcache_get(&kvcache_head_ring[ack_hash_0%MAX_AREA_SIZE], priority, ack_hash_1, ack_hash_2);
	if(NULL == kv_node) return NULL;
	
	long now = (long_t)time((time_t *)NULL);

	/*timeout*/
	if(now > kv_node->tms + kv_node->ttl)
	{
		return NULL;
	}

	return kv_node->value;
}

int kvcache_del(const char *key, ulong_t klen)
{
	ulong_t  ack_hash_0 = hash_string(key, klen, 0);
	ulong_t  ack_hash_1 = hash_string(key, klen, 1);
	ulong_t  ack_hash_2 = hash_string(key, klen, 2);

	ulong_t priority = ack_hash_0/MAX_AREA_SIZE;

	 return  __kvcache_del(&kvcache_head_ring[ack_hash_0%MAX_AREA_SIZE], priority, ack_hash_1, ack_hash_2);
}

int kvcache_ttl(const char *key, ulong_t klen)
{
	ulong_t  ack_hash_0 = hash_string(key, klen, 0);
	ulong_t  ack_hash_1 = hash_string(key, klen, 1);
	ulong_t  ack_hash_2 = hash_string(key, klen, 2);

	ulong_t priority = ack_hash_0/MAX_AREA_SIZE;

	kvcache_node *kv_node = __kvcache_get(&kvcache_head_ring[ack_hash_0%MAX_AREA_SIZE], priority, ack_hash_1, ack_hash_2);
	if(NULL == kv_node) return NULL;

	long now = (long_t)time((time_t *)NULL);

	int ttl = now - kv_node->tms;

	return (ttl > 0? ttl:-1);
}

int kvcache_expire(const char *key, ulong_t klen, long_t ttl)
{
	ulong_t  ack_hash_0 = hash_string(key, klen, 0);
	ulong_t  ack_hash_1 = hash_string(key, klen, 1);
	ulong_t  ack_hash_2 = hash_string(key, klen, 2);

	ulong_t priority = ack_hash_0/MAX_AREA_SIZE;

	kvcache_node *kv_node = __kvcache_get(&kvcache_head_ring[ack_hash_0%MAX_AREA_SIZE], priority, ack_hash_1, ack_hash_2);
	if(NULL == kv_node) return NULL;

	long now = (long_t)time((time_t *)NULL);

	/*timeout*/
	if(now > kv_node->tms + kv_node->ttl)
	{
		return -1;
	}

	kv_node->tms = now;
	kv_node->ttl = ttl;

	return 0;
}


