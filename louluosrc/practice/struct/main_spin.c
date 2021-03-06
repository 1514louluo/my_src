#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "doublequeue_spin.h"

#define TOTAL_COUNT 10000000
#define AVG_COUNT   (TOTAL_COUNT/10)

static void free_elem(void *data)
{
	P_FREE(data);
}

static void* consumer(void* arg)
{
	struct double_queue_head *head = (struct double_queue_head*)arg;

	int count = 0;
	int sum = 0;
	int i = 0;
	while(1)
	{
		++sum;
		void *data = double_queue_get(head);
		if(data)
		{
			int *x = (int *)(data);
			if(0 == count%AVG_COUNT)
				printf("%d\n", *x);
			free_elem(data);
			++count;
		}

		if(TOTAL_COUNT == count)
		{
			printf("count = %d\n", count);
			printf("sum = %d\n", sum);
			break;
		}

	}


	return arg;
}

static void* producer(void* arg)
{
	struct double_queue_head *head = (struct double_queue_head*)arg;

	int i = 0;
	for(; i < TOTAL_COUNT; ++i)
	{
		int *x = (int *)malloc(4);
		*x = i;
		double_queue_put(head, x);
	}

	return arg;
}
int main()
{

	struct timeval tv_s;
	struct timeval tv_e;
	pthread_spinlock_t lock;
	sem_t           cond;
	pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
	sem_init(&cond, 0, 0);
	struct double_queue_head *head = double_queue_init(&lock, &cond);

	gettimeofday(&tv_s, NULL);
	pthread_t producer_id, consumer_id;
	pthread_create(&producer_id, NULL, producer, head);
	pthread_create(&consumer_id, NULL, consumer, head);

	pthread_join(producer_id, NULL);
	pthread_join(consumer_id, NULL);

	gettimeofday(&tv_e, NULL);
	printf("use sec: %lu\t", tv_e.tv_sec - tv_s.tv_sec);
	printf("use usec:%lu\n", tv_e.tv_usec - tv_s.tv_usec);

	double_queue_free(head, free_elem);

	return 0;
}
