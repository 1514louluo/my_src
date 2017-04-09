/*
 * block_queue.h
 *
 *  Created on: Sep 26, 2011
 *      Author: hehenpan
 */

#ifndef BLOCK_QUEUE_H_
#define BLOCK_QUEUE_H_

#include "pthread.h"
#include "stdlib.h"
#include "stdio.h"
#include <list>
#include "mycommon.h"

template<typename T>
class block_queue {
public:
	block_queue()
	{
		pthread_mutex_init(&m_thread_mutex, NULL);
		pthread_cond_init(&m_thread_cond, NULL);
		pthread_mutex_init(&m_list_mutex, NULL);
		m_seconds = 3;
		m_size = 0;
	}
	void set_timeout(int seconds)
	{
		m_seconds =  seconds;
	}
	virtual ~block_queue() {}

	std::list<T> m_list;

	//typedef std::list<T>::iterator q_iterator;

	pthread_mutex_t   m_thread_mutex;
	pthread_cond_t    m_thread_cond;
	pthread_mutex_t   m_list_mutex;
	int m_seconds;
	int m_size;
	//template<typename T>
	int add_queue(T data)
	{
		{
			scopedlock lock2(&m_list_mutex);
			m_list.push_back(data);
			//printf("add queue,size=%d\n", m_list.size());
			m_size++;
		}
		pthread_cond_signal(&m_thread_cond);
		
		return 0;
	}

	/* return value:
	 *  -1  error
	 *  0   no data retrived
	 *  1   on data retrived
	 */
	//template<typename T>
	int dequeue(T &data)
	{

		//printf("dequeue size=%d\n",m_list.size());
		scopedlock lock1(&m_thread_mutex);
		timespec tsp;
		maketimeout_seconds(&tsp, m_seconds);
		int result = pthread_cond_timedwait(&m_thread_cond, &m_thread_mutex, &tsp);
		switch(result)
		{
		case 0:
			break;
		case ETIMEDOUT:
			break;
		default:
			//PRINTLOG_ERROR("block_queue condition wait failed with error %d, error str=%s", result, strerror(result));
			return -1;
			break;
		}
		lock1.unlock();

		scopedlock lock2(&m_list_mutex);
		if(!m_list.empty())
		{
			
			data = m_list.front();
			m_list.pop_front();
			m_size--;
			return 1;

		}
		else
		{
			return 0;
		}


	}

	int dequeue_nodelay(T &data)
	{
		//printf("dequeue_nodelay size=%d\n",m_list.size());
		scopedlock lock2(&m_list_mutex);
		if(! m_list.empty())
		{
			data = m_list.front();
			m_list.pop_front();
			m_size--;
			return 1;
			
		}
		else
		{
			return 0;
		}

	}

	int getsize(void)
	{
		scopedlock lock2(&m_list_mutex);
		return m_size;
	}
	int dequeue_expand(T &data)
	{
		//printf("dequeue_expand size=%d\n",m_list.size());
		int result;
		result = dequeue_nodelay(data);   // 先探测队列中是否有数据
		if (result == 0)
		{ // 如果没有数据，则阻塞等待
			result = dequeue(data);
			return result;
		}
		else
		{  // 如果有数据，则直接取出来，并立即返回
			return result;
		}
		
	}
};



#endif /* BLOCK_QUEUE_H_ */










