/*
*	A simple kernel FIFO implementation. modify from kernel kfifo.
*	License: LGPL 
* 	modify by louluo! <18915413902@163.com> if there is some bug, please let me know!
*/
#ifndef _LINUX_KFIFO_H
#define _LINUX_KFIFO_H

#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define P_MALLOC malloc      /* here can use jemalloc, improve performance 1/3 */
#define P_FREE   free        /* here can use jemalloc, improve performance 1/3 */

#ifdef __cplusplus
extern "C"
{
#endif

/* is x a power of 2? */
#define is_power_of_2(x)	((x) != 0 && (((x) & ((x) - 1)) == 0))      /* is the number pow of 2? */
#define BUG_ON(x)		assert(!(x))                                /* assert x is false/NULL/0 */
#define min(a, b) (((a) < (b)) ? (a) : (b))                                 /* who is min between a and b? */

static inline int fls(unsigned int  x)         /* find last set 1bit from low to high */
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

#define roundup_pow_of_two(n)     (((n) == 0) ? 0:(1<<(((n) >= (1<<31))?31:((fls(n)+1)))))   /* a number to pow 2 */

struct kfifo
{
        unsigned char *buffer;		/* the buffer holding the data */
        unsigned int size;		/* the size of the allocated buffer */
        unsigned int in;		/* data is added at offset (in % size) */
        unsigned int out;		/* data is extracted from off. (out % size) */
        pthread_spinlock_t *lock;	/* protects concurrent modifications */
};


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


