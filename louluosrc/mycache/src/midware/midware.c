#include "midware.h"

static struct kfifo *request_fifo;
static struct kfifo *response_fifo;


/*
*	request fifo ref opts!
*/
int request_fifo_init(unsigned int size, pthread_spinlock_t *lock)
{
	request_fifo = kfifo_alloc(size, lock);
	return ((request_fifo)?0:-1)
}

void request_fifo_free(void)
{
	kfifo_free(request_fifo);
}

unsigned int request_fifo_put(const unsigned char * buffer, unsigned int len)
{
	return kfifo_put(request_fifo, buferror, len);
}

unsigned int request_fifo_get(unsigned char *buffer, unsigned int len)
{
	return kfifo_get(request_fifo,  buffer, len);
}

void request_fifo_reset(void)
{
	kfifo_reset(request_fifo);
}

unsigned int request_fifo_len(void)
{
	return kfifo_len(request_fifo);
}



/*
*	response fifo ref opts!
*/

int response_fifo_init(unsigned int size, pthread_spinlock_t *lock)
{
	response_fifo = kfifo_alloc(size, lock);
	return ((response_fifo)?0:-1)
}

void response_fifo_free(void)
{
	kfifo_free(response_fifo);
}

unsigned int response_fifo_put(const unsigned char * buffer, unsigned int len)
{
	return kfifo_put(response_fifo, buferror, len);
}

unsigned int response_fifo_get(unsigned char *buffer, unsigned int len)
{
	return kfifo_get(response_fifo,  buffer, len);
}

void response_fifo_reset(void)
{
	kfifo_reset(response_fifo);
}

unsigned int response_fifo_len(void)
{
	return kfifo_len(response_fifo);
}



