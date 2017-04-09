#ifndef _MIDWARE_H
#define _MIDWARE_H

#include "kfifo.h"

/*
*	request fifo ref opts!
*/
extern int request_fifo_init(unsigned int size, pthread_spinlock_t *lock);


extern void request_fifo_free(void);


extern unsigned int request_fifo_put(const unsigned char * buffer, unsigned int len);


extern unsigned int request_fifo_get(unsigned char *buffer, unsigned int len);


extern void request_fifo_reset(void);


extern unsigned int request_fifo_len(void);




/*
*	response fifo ref opts!
*/

extern int response_fifo_init(unsigned int size, pthread_spinlock_t *lock);

extern void response_fifo_free(void);


extern unsigned int response_fifo_put(const unsigned char * buffer, unsigned int len);


extern unsigned int response_fifo_get(unsigned char *buffer, unsigned int len);


extern void response_fifo_reset(void);


extern unsigned int response_fifo_len(void);





#endif //_MIDWARE_H

