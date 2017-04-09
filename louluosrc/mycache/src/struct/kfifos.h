/*
 * A simple kernel FIFO implementation.
 *
 * modify by louluo!
 */
#ifndef _KFIFOS_H
#define _KFIFOS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <pthread.h>
#include <string.h>
#include <assert.h>
#include "cdef.h"

/* is x a power of 2? */
#define is_power_of_2(x)	((x) != 0 && (((x) & ((x) - 1)) == 0))
#define BUG_ON(x)		assert(!(x))
#define min(a, b) (((a) < (b)) ? (a) : (b))

static inline int fls(unsigned int  x)
{
        int r = 0;

        if(!x)
                return 0;
        if(x & 0xffff0000)
        {
                x >>= 16;
                r += 16;
        }
        if(x & 0xff00)
        {
                x >>= 8;
                r += 8;
        }
        if(x & 0xf0)
        {
                x >>= 4;
                r += 4;
        }
        if(x & 0xc)
        {
                x >>= 2;
                r += 2;
        }
        if(x & 0x2)
        {
                x >>= 1;
                r += 1;
        }
        return r;
}

#define roundup_pow_of_two(n)     (((n) == 0) ? 0:(1<<(((n) >= (1<<31))?31:((fls(n)+1)))))   

struct kfifo
{
	    struct kfifo *next;       /*this is slist node*/
        unsigned char *buffer;	/* the buffer holding the data */
        unsigned int size;	/* the size of the allocated buffer */
        unsigned int in;	/* data is added at offset (in % size) */
        unsigned int out;	/* data is extracted from off. (out % size) */
};

struct kfifos
{
	struct kfifo *busy;    //busy and free
    struct kfifo *idle;    //busy and free
	struct kfifo *busy_in;
	struct kfifo *busy_out;
	int       busy_count;
	int       idle_count;
    pthread_spinlock_t *lock; /* protects concurrent modifications */
};


extern struct kfifo_head_init();
extern kfifos_put(struct kfifo_head *head, 
					const unsigned char *buffer, unsigned int len);
extern kfifos_get(struct kfifo_head *head, 
					const unsigned char *buffer, unsigned int len);



extern struct kfifo *kfifo_alloc(unsigned int size, pthread_spinlock_t *lock);

extern void kfifo_free(struct kfifo *fifo);

extern unsigned int kfifo_put(struct kfifo *fifo,
                                     const unsigned char *buffer, unsigned int len);

extern unsigned int kfifo_get(struct kfifo *fifo,
                                     unsigned char *buffer, unsigned int len);

extern unsigned int kfifo_len(struct kfifo *fifo);

extern void kfifo_reset(struct kfifo *fifo);


#ifdef __cplusplus
}
#endif

#endif


