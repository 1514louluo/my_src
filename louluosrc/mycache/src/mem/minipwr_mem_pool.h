#ifndef _MINIPWR_MEM_POOL_H
#define _MINIPWR_MEM_POOL_H
#include "bitmap.h"
#include "rbtree.h"

#include "logger.h"
#ifdef _DEBUG
DECL_LOGGER(sys_logger);
#endif


#define DIV_ROUND_UP_FOR_8BYTES(n) (((n) + 0X7)>>3)

#ifdef _CPU_32BIT
#define MEM_BITMAP_SIZE                           16                     // 64 bytes
#else
#define MEM_BITMAP_SIZE                           8                     // 64 bytes
#endif

#define MEM_NODE_ALLOC_BLOCK                1024                    // 1024 bytes
#define MEM_NODE_BODY_SIZE        (MEM_NODE_ALLOC_BLOCK<<2)   // 4k bytes

#define MEM_POOL_SIZE                   sizeof(minipwr_mem_pool_t)
#define MEM_NODE_SIZE                   sizeof(minipwr_mem_node_t)

typedef struct minipwr_mem_node minipwr_mem_node_t;
typedef struct minipwr_mem_pool minipwr_mem_pool_t;

typedef struct rb_node RB_node_t;
typedef struct rb_root RB_root_t;
typedef int (*pool_expand_func_t)(minipwr_mem_pool_t *);


struct minipwr_mem_node
{
        RB_node_t        node;
        ulong_t            bitmap_t[MEM_BITMAP_SIZE];
        void                 *preal; //just minipwr_mem_head pmem_start
} __attribute__((aligned(sizeof(long_t))));


struct minipwr_mem_pool
{
        minipwr_mem_node_t            *current_series_node;
        minipwr_mem_node_t            *latest_free_node;

        void                             *current_series_ptr;
        void                             *latest_free_ptr;

        ulong_t            		pool_total_size;
        ulong_t            		pool_left_series_size;
        ulong_t            		pool_node_nb;

        RB_root_t   	   		          rbroot;
        pool_expand_func_t                expand_node;

        pthread_spinlock_t                  spin_lock;
} __attribute__((aligned(sizeof(long_t))));

//int minipwr_expand_node(minipwr_mem_pool_t *ppool);

int minipwr_create_pool(int size);
void minipwr_destroy_pool(void);

static void *minipwr_palloc(int howmany);
static void *minipwr_pcalloc(int howmany);
static void minipwr_pfree(void *p, int howmany);

#define  mini_palloc(size)     minipwr_palloc(DIV_ROUND_UP_FOR_8BYTES(size))
#define  mini_pcalloc(size)    minipwr_pcalloc(DIV_ROUND_UP_FOR_8BYTES(size))
#define  mini_pfree(p)                                                                                                           \
	do{                                                                                                                                 \
		int howmany = DIV_ROUND_UP_FOR_8BYTES(sizeof(*p));                                        \
		minipwr_pfree(p, howmany);                                                                                   \
	}while(0);

#endif //_MINIPWR_MEM_POOL_H
