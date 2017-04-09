#include <stdio.h>
#include <time.h>
#include <sys/time.h>
//#include "doublequeue.h"
#include <semaphore.h>

#include <stdlib.h>
//#include "jemalloc"
#define P_MALLOC malloc      /* here can use jemalloc, improve performance 1/3 */
#define P_FREE   free        /* here can use jemalloc, improve performance 1/3 */

#define TOTAL_COUNT 10000000
#define AVG_COUNT   (TOTAL_COUNT/10)

static void free_elem(void *data)
{
	P_FREE(data);
}

static void* consumer(void* arg)
{
	sleep(2);
	printf("consumer\n");
	printf("consumer begin!\n");
	sem_t *cond = (sem_t *)arg;

	int i = 0;
	for(; i < 12; ++i)
	{
		sem_wait(cond);
		printf("hello world!\n");
	}
	return arg;
}

static void* producer(void* arg)
{
	printf("producer\n");
	sem_t *cond = (sem_t *)arg;

	int i = 0;
	for(; i < 8; ++i)
	{
		sem_post(cond);
	}
	printf("producer finish!\n");
	sleep(100);

	return arg;
}
int main()
{

	struct timeval tv_s;
	struct timeval tv_e;
	sem_t           cond;
	sem_init(&cond, 0, 0);

	gettimeofday(&tv_s, NULL);
	pthread_t producer_id, consumer_id;
	pthread_create(&producer_id, NULL, producer, &cond);
	pthread_create(&consumer_id, NULL, consumer, &cond);

	pthread_join(producer_id, NULL);
	pthread_join(consumer_id, NULL);

	gettimeofday(&tv_e, NULL);
	printf("use sec: %lu\t", tv_e.tv_sec - tv_s.tv_sec);
	printf("use usec:%lu\n", tv_e.tv_usec - tv_s.tv_usec);


	return 0;
}
