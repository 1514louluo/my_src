#include "minipwr_mem_pool.h"
void *minipwr_alloc(size_t size, minipwr_log_t *log)
{
	void *p;

	p = malloc(size);
	if(NULL == p)
	{
		//for log
	}
	//log
	return p;
}
void *minipwr_calloc(size_t size, minipwr_log_t *log)
{
	void *p;
	p = minipwr_alloc(size, log);
	if(p) memset(p, 0, size);
	
	return p;
}
void *minipwr_memalign(size_t alignment, size_t size, minipwr_log_t *log)
{
    void  *p;
    int    err;

    err = posix_memalign(&p, alignment, size);
    if (err) 
	{
		//for log
		p = NULL;
	}
	//log
    return p;
}

minipwr_mem_pool_t *minipwr_create_pool(size_t size, minipwr_log_t *log)
{
	minipwr_mem_pool_t *p;

    p = minipwr_memalign(ALLOC_ALIGNMENT, size, log);
    if (p == NULL) {
        return NULL;
    }

    p->current = p;
	p->next = NULL;
	p->bmem = NULL;
    p->log = log;
	p->data = NULL;

	p->smem.begin = (uint8_t *)p + sizeof(minipwr_mem_pool_t);
	p->smem.end = (uint8_t *)p + size - 1;
	p->smem.current = p->smem.begin;
	p->smem.max_size = size - sizeof(minipwr_mem_pool_t);
	p->smem.left_series_size = p->smem.max_size;
	p->smem.lsmem_begin_pos = p->smem.begin;
	bitmap_zero(p->smem.bitmap_t16, 2 * BITS_PER_LONG);
	bitmap_zero(p->smem.bitmap_t8 , 4 * BITS_PER_LONG);

    return p;
}
void minipwr_destroy_pool(minipwr_mem_pool_t *pool)
{
}
void minipwr_reset_pool(minipwr_mem_pool_t *pool)
{
}

static void *minipwr_palloc_big(minipwr_mem_pool_t *pool, size_t size, size_t flag, minipwr_log_t *log)
{
	minipwr_big_mem_t *bmem = (minipwr_big_mem_t *)minipwr_alloc(sizeof(minipwr_big_mem_t), log);
	if(ALLOC_TYPE == flag)
		bmem->alloc = minipwr_alloc(size, log);
	else
		bmem->alloc = minipwr_calloc(size, log);
	bmem->cur_size = size;
	bmem->flag = BMEM_ALIVE;

	bmem->next = pool->bmem->next;
	pool->bmem->next = bmem;

	return bmem;
}	

static void *minipwr_palloc_small(minipwr_mem_pool_t *pool, size_t size, size_t flag, minipwr_log_t *log)
{
	if(0 >= size)	return NULL;
	/*����pool��small map�����Ƿ���������С�ڴ�����ṩ*/
	else if(8 >= size)
	{
		int i;
		for(i = 1; i <= 120; i++)	
		{
			if(!bitmap_find(pool->smem.bitmap_t8, i))
			{
				pool->smem.current = pool->smem.begin + (i-1) * 8;
				bitmap_cpl(pool->smem.bitmap_t8, i);
				if(!bitmap_find(pool->smem.bitmap_t16, (i+1)>>1))	
					bitmap_cpl(pool->smem.bitmap_t16, (i+1)>>1);

				if(CALLOC_TYPE == flag)
					memset(pool->smem.current, 0, size);
				return pool->smem.current;
			}
		}
	}
		/*����pool��big map�����Ƿ���������С�ڴ�����ṩ*/
	else if(16 >= size)
	{
		int i;
		for(i = 1; i <= 60; i++)	
		{
			if(!bitmap_find(pool->smem.bitmap_t16, i))
			{
				pool->smem.current = pool->smem.begin + (i-1) * 16;
				bitmap_cpl(pool->smem.bitmap_t16, i);

				if(CALLOC_TYPE == flag)
					memset(pool->smem.current, 0, size);
				return pool->smem.current;
			}
		}
	}
		/*����pool��big map�����Ƿ���������С�ڴ�����ṩ*/
	else if(pool->smem.max_size >= size)
	{
		int range = DIV_ROUND_UP(size, ALLOC_ALIGNMENT);		
		/*need a al*/
		int i, start = 1;
		for(i = 1; i <= 60; i++)	
		{
			if(range == (i - start + 1))
				break;
			if(bitmap_find(pool->smem.bitmap_t16, i))
				start = i+1;
		}
		if(60 == i)
		{
			for(i = 0; i < range; i++)
				bitmap_cpl(pool->smem.bitmap_t16, start+i);
			pool->smem.current = pool->smem.begin + (i-1) * 16;

			if(CALLOC_TYPE == flag)
				memset(pool->smem.current, 0, size);
			return pool->smem.current;
		}
	}
		 /*������ڴ�*/
	else
		return NULL;
	/*������϶�û�к��ʵ�*/
	if(pool->smem.left_series_size > size)
		 //���Ȳ鿴pool->smem.left_series_size�Ƿ����size������������
	{
			pool->smem.current = pool->smem.lsmem_begin_pos;
			size_t b_index = (pool->smem.current - pool->smem.begin) / ALLOC_ALIGNMENT + 1;
			size_t s_index = (pool->smem.current - pool->smem.begin) / MIN_ALIGNMENT + 1;

			assert(!bitmap_find(pool->smem.bitmap_t16, b_index));
			assert(!bitmap_find(pool->smem.bitmap_t8, s_index));

			int b_range = DIV_ROUND_UP(size, ALLOC_ALIGNMENT);
			int s_range = DIV_ROUND_UP(size, MIN_ALIGNMENT);

			int i;
			for(i = 0; i < b_range; i++)
				bitmap_cpl(pool->smem.bitmap_t16, b_index);
			for(i = 0; i < s_range; i++)	
				bitmap_cpl(pool->smem.bitmap_t8,  s_index);

			pool->smem.lsmem_begin_pos += b_range;
				
			if(CALLOC_TYPE == flag)
				memset(pool->smem.current, 0, size);
			return pool->smem.current;
	}
	/*������ϻ���û�к��ʵ�*/
	if(NULL != pool->next)
		 //������һ��pool���ظ�һ�η������
		return minipwr_palloc_small(pool->next, size);

	/*������ϻ���û�к��ʵ� ������ڴ�*/
	return NULL;
}

void *minipwr_palloc(minipwr_mem_pool_t *pool, size_t size, minipwr_log_t *log)
{
	if(minipwr_palloc_small(pool, size, ALLOC_TYPE, log))
		return pool->smem.current;
	else
		return minipwr_palloc_big(pool, size, ALLOC_TYPE, log);
}

void *minipwr_pcalloc(minipwr_mem_pool_t *pool, size_t size, minipwr_log_t *log)
{
	if(minipwr_palloc_small(pool, size, CALLOC_TYPE, log))
		return pool->smem.current;
	else
		return minipwr_palloc_big(pool, size, CALLOC_TYPE, log);
}

int minipwr_pfree(minipwr_mem_pool_t *pool, void *p, size_t size)
{
	/*������pool�������ҵ�p�Ƿ�������ĳ����smem��*/
	minipwr_mem_pool_t *index_pool = pool;
	while(index_pool)
	{
		if(p <= index_pool->smem.end && p >= index_pool->smem.begin)		
			break;
	}
	if(index_pool)
	/*��ĳ��pool��smem���ҵ���*/
	{
		/*��size��end-p��Ƚϣ���size�Ƿ�Ϸ�*/
	}
	/*��ʱ��Ҫ����pool�����е�bmem*/
	index_pool = pool;
	while(index_pool)
	{
		minipwr_big_mem_t *index_bmem = index_pool->bmem;
		while(index_bmem)
		{
			if(p == index_bmem->alloc)
			{
				if(size != index_bmem->cur_size || BMEM_DIE == index_bmem->flag)
				{
					/*size���Ϸ�*/
				}
				else
				{
					/*�ҵ���*/
				}
			}
		}
	}
}









