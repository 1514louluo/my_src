#ifndef _MINIPWR_MEM_POOL_H
#define _MINIPWR_MEM_POOL_H
#include "minipwr_global.h"
#include "minipwr_bit_map.h"
/*
* The bitmap just like follow:
*          1     0     0     1 ...  bitmap_t16[2]
*         / \   / \   / \   / \
*        1   0 0   0 0   0 0   1 ... bitmap_t8[4]
* */
#define BMEM_ALIVE  1
#define BMEM_DIE    0 

#define ALLOC_TYPE   1
#define CALLOC_TYPE  0
typedef struct minipwr_small_mem
{
		void *current;
		void *begin;
		void *end;
		size_t max_size;
		void *lsmem_begin_pos;
		size_t left_series_size;
		uint32_t bitmap_t16[2]; /*if pool size is 1024,remind 960 bytes,960/16=60, 4 bits is unuseful*/
		uint32_t bitmap_t8[4];  /*if pool size is 1024,remind 960 bytes,960/8=120, 8 bits is unuseful*/
} minipwr_small_mem_t;

typedef struct minipwr_big_mem
{
		void *alloc;
		size_t cur_size;
		size_t flag;
		struct minipwr_big_mem *next;
} minipwr_big_mem_t;

typedef struct minipwr_log_s {
/*    minipwr_uint_t           log_level;
    minipwr_open_file_t     *file;

    minipwr_atomic_uint_t    connection;

    minipwr_log_handler_pt   handler;
 */   void                *data;
    char                *action;
} minipwr_log_t;

typedef struct minipwr_mem_pool
{
		void *current;
		struct minipwr_mem_pool *next;
		minipwr_big_mem_t *bmem;
		minipwr_log_t *log;
//		void *data; //someone else
		minipwr_small_mem_t smem;
} minipwr_mem_pool_t;


void *minipwr_alloc(size_t size, minipwr_log_t *log);
void *minipwr_calloc(size_t size, minipwr_log_t *log);
void *minipwr_memalign(size_t alignment, size_t size, minipwr_log_t *log);

minipwr_mem_pool_t *minipwr_create_pool(size_t size, minipwr_log_t *log);
void minipwr_destroy_pool(minipwr_mem_pool_t *pool);
void minipwr_reset_pool(minipwr_mem_pool_t *pool);

void *minipwr_palloc(minipwr_mem_pool_t *pool, size_t size);
void *minipwr_pcalloc(minipwr_mem_pool_t *pool, size_t size);
int minipwr_pfree(minipwr_mem_pool_t *pool, void *p);

#endif //_MINIPWR_MEM_POOL_H
