#include "minipwr_mem_pool.h"
minipwr_mem_pool_t *mem_pool;

static void *minipwr_stdalloc(int size)
{
        void *p = P_MALLOC(size);
        return p;
}
static void *minipwr_stdcalloc(int size)
{
        void *p;
        p = minipwr_stdalloc(size);
        if(p) memset(p, 0, size);
        return p;
}

static void minipwr_stdfree(void *p)
{
        P_FREE(p);
}

static void *minipwr_memalign(int alignment, int size)
{
        void  *p;
        int    err =  posix_memalign(&p, alignment, size);
        if(err)
        {
                p = NULL;
        }
        return p;
}

static void *minipwr_init_mem_node(void)
{
        minipwr_mem_node_t *p;

        p = minipwr_stdalloc(MEM_NODE_SIZE);
        if(p == NULL)
        {
                SYS_LOG_ERROR("minipwr_stdalloc error!");
                return NULL;
        }

        p->preal = minipwr_memalign(ALLOC_ALIGNMENT, MEM_NODE_BODY_SIZE);
        if(NULL == p->preal)
        {
                SYS_LOG_ERROR("minipwr_memalign error!");
                return NULL;
        }

        bitmap_zero(p->bitmap_t,  MEM_BITMAP_SIZE * BITS_PER_LONG);

        return p;
}

static minipwr_mem_node_t *minipwr_mem_node_search(RB_root_t *root, void *pindex)
{
        RB_node_t *node = root->rb_node;
        while(node)
        {
                minipwr_mem_node_t *data = container_of(node, minipwr_mem_node_t, node);
                if(pindex < data->preal) node = node->rb_left;
                else if(pindex > data->preal + MEM_NODE_BODY_SIZE) node = node->rb_right;
                else return data;
        }
        return NULL;
}

static int minipwr_mem_node_insert(RB_root_t *root, minipwr_mem_node_t *data)
{
        RB_node_t **new = &(root->rb_node), *parent = NULL;
        /* Figure out where to put new node */
        while(*new)
        {
                minipwr_mem_node_t *this = container_of(*new, minipwr_mem_node_t, node);
                parent = *new;
                if(data->preal < this->preal) new = &((*new)->rb_left);
                else if(data->preal > this->preal + MEM_NODE_BODY_SIZE) new = &((*new)->rb_right);
                else return 0;
        }
        /* Add new node and rebalance tree. */
        rb_link_node(&data->node, parent, new);
        rb_insert_color(&data->node, root);
        return 1;
}

static void minipwr_mem_node_del(RB_root_t *root, void *pindex)
{
        minipwr_mem_node_t *data = minipwr_mem_node_search(root, pindex);
        if(data)
        {
                rb_erase(&data->node, root);
                minipwr_stdfree(data);
        }
}

static void minipwr_mem_node_foreach(RB_root_t *root)
{
        RB_node_t *node;
        for(node = rb_first(root); node; node = rb_next(node))
                printf("key=%p\t", rb_entry(node, minipwr_mem_node_t, node)->preal);
}


int minipwr_create_pool(int num)
{
        SYS_LOG_INFO("<===========create mem pool!============>");
        if(num <= 0)
        {
                SYS_LOG_ERROR("mem pool size lte 0! size = %d", num);
                return -1;
        }
        mem_pool = minipwr_stdalloc(MEM_POOL_SIZE);

        mem_pool->pool_total_size = num * MEM_NODE_BODY_SIZE;
        mem_pool->pool_left_series_size = mem_pool->pool_total_size;
        mem_pool->pool_node_nb = num;

        mem_pool->expand_node = NULL;
        mem_pool->rbroot = RB_ROOT;

        int i = 0;
        for(; i < num; ++i)
        {

                minipwr_mem_node_t  *pnode =  minipwr_init_mem_node();
                if(NULL == pnode)
                {
                        SYS_LOG_ERROR("minipwr_init_mem_node error!");
                        return -1;
                }
                minipwr_mem_node_insert(&mem_pool->rbroot, pnode);
                if(0 == i)
                {
                        mem_pool->current_series_node = pnode;
                        mem_pool->latest_free_node =  pnode;
                        mem_pool->current_series_ptr = pnode->preal;
                        mem_pool->latest_free_ptr = pnode->preal;
                }
        }

        if(pthread_spin_init(&mem_pool->spin_lock , PTHREAD_PROCESS_PRIVATE))
        {
                SYS_LOG_ERROR("spin lock init error!");
                return -1;
        }

        SYS_LOG_INFO("<===========create mem pool finist!============>");
        return 0;
}

void minipwr_destroy_pool(void)
{
        SYS_LOG_INFO("<===========destroy mem pool!============>");
        RB_node_t *node  = rb_first(&mem_pool->rbroot);
        while(node)
        {
                minipwr_mem_node_t *data = container_of(node, minipwr_mem_node_t, node);
                minipwr_stdfree(data->preal);
                RB_node_t *tmp = node;
                node = rb_next(node);
                rb_erase(tmp, &mem_pool->rbroot);
        }
        minipwr_stdfree(mem_pool);
        SYS_LOG_INFO("<===========destroy mem pool finish!============>");
}

static void *minipwr_palloc(int howmany)
{
        int size = howmany<<3;
        if(NULL == mem_pool)
        {
                SYS_LOG_ERROR("mem_pool ptr is nil! please check whether init!");
                return NULL;
        }

        if(MEM_NODE_BODY_SIZE < size)
        {
                SYS_LOG_WARN("size is larger than MEM_NODE_BODY_SIZE(4096)! size = %d", size);
                return minipwr_stdalloc(size);
        }

        else if(0 >= size)
        {
                return NULL;
        }

        /*sure that size has how many 8bytes*/
        void *latest_free_ptr = mem_pool->latest_free_ptr;
        void *current_series_ptr = mem_pool->current_series_ptr;
        /* first check latest_free_ptr */
        minipwr_mem_node_t *mem_node =  mem_pool->latest_free_node;
        ulong_t *mem_node_bitmap = mem_node->bitmap_t;

        int pos = DIV_ROUND_UP_FOR_8BYTES(latest_free_ptr - mem_node->preal);
        if(0 == bitmap_isset_region(mem_node_bitmap, pos, howmany))
        {
                bitmap_setbit_region(mem_node_bitmap, pos, howmany);
                if(latest_free_ptr == current_series_ptr)
                {
                        mem_pool->current_series_ptr += size;
                        mem_pool->pool_left_series_size -= size;
                }
                return latest_free_ptr;
        }
        /* second check current_series_ptr*/
        mem_node =  mem_pool->current_series_node;
        mem_node_bitmap = mem_node->bitmap_t;
        pos = current_series_ptr - mem_node->preal;
        if(pos + size < MEM_NODE_BODY_SIZE)
        {
                pos = DIV_ROUND_UP_FOR_8BYTES(pos);
                bitmap_setbit_region(mem_node_bitmap, pos, howmany);
                mem_pool->current_series_ptr += size;
                mem_pool->pool_left_series_size -= size;
                if(latest_free_ptr == current_series_ptr)
                {
                        mem_pool->latest_free_ptr += size;
                }
                return current_series_ptr;
        }
        /* third scan current_series_node, search next node */
        RB_node_t *next_rb_node  = rb_next(&mem_node->node);

        if(next_rb_node)
        {
                mem_node = container_of(next_rb_node, minipwr_mem_node_t, node);
                mem_node_bitmap = mem_node->bitmap_t;
                bitmap_setbit_region(mem_node_bitmap, 0, howmany);
                mem_pool->current_series_node = mem_node;
                mem_pool->current_series_ptr = mem_node->preal + size;
                mem_pool->pool_left_series_size -= size;


                return mem_node->preal;
        }
        /* last all node can not alloc, insert a new node */
        minipwr_mem_node_t  *new_pnode = minipwr_init_mem_node();
        if(NULL == new_pnode)
        {
                SYS_LOG_ERROR("minipwr_init_mem_node error!");
                return NULL;
        }

        mem_node_bitmap = new_pnode->bitmap_t;
        bitmap_setbit_region(mem_node_bitmap, 0, howmany);

        minipwr_mem_node_insert(&mem_pool->rbroot, new_pnode);

        mem_pool->current_series_node = new_pnode;
        mem_pool->current_series_ptr = new_pnode->preal + size;
        mem_pool->pool_left_series_size += MEM_NODE_SIZE - size;
        mem_pool->pool_total_size += MEM_NODE_SIZE;
        mem_pool->pool_node_nb += 1;

        return new_pnode->preal;
}
static void *minipwr_pcalloc(int size)
{
        void *p = minipwr_palloc(size);
        if(p)
                memset(p, 0, size);
        return 0;
}
static void minipwr_pfree(void *p, int howmany)
{
        int size = howmany<<3;
        if(NULL == p) return;
        /* index in rb_tree */
        minipwr_mem_node_t  *mem_node = minipwr_mem_node_search(&mem_pool->rbroot, p);
        if(NULL == mem_node)
        {
                minipwr_stdfree(p);
        }
        else
        {
                int pos = DIV_ROUND_UP_FOR_8BYTES(p - mem_node->preal);
                ulong_t *latest_free_node_bitmap = mem_node->bitmap_t;

                bitmap_clrbit_region(latest_free_node_bitmap, pos, howmany);
                mem_pool->latest_free_node = mem_node;
                mem_pool->latest_free_ptr = p;

                if(mem_pool->current_series_ptr == p + size)
                {
                        mem_pool->current_series_ptr = p;
                        mem_pool->pool_left_series_size += size;
                }
        }
}



