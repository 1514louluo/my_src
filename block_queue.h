/* author: manialuo
 * */
#ifndef _BLOCKQUEUE_H_
#define _BLOCKQUEUE_H_

#include "scoped_lock.h"
#include <list>

#define DEFAULT_MAX_QUEUE_SIZE 100000
#define DEFAULT_TIMEOUT        1

template<typename T>
class BlockQueue {
  public:
    BlockQueue()
    {
      pthread_mutex_init(&m_list_mutex, NULL);
      pthread_cond_init(&m_cond_noempty, NULL);
      pthread_cond_init(&m_cond_nofull, NULL);
      m_max_size = DEFAULT_MAX_QUEUE_SIZE;
      m_seconds = DEFAULT_TIMEOUT;
      m_size = 0;
    }

    virtual ~BlockQueue(){}
    
    void setTimeout(int seconds)
    {
      m_seconds =  seconds;
    }

    void setMaxQueueSize(int size)
    {
      m_max_size =  size;
    }


    int addqueue(T &data)              //只提供阻塞操作暴露给生产者
    {
      ScopedLock lock(&m_list_mutex);  //增加时首先考虑是否已经满了
      while(m_max_size <= m_size)
        pthread_cond_wait(&m_cond_nofull, &m_list_mutex); 

      m_list.push_back(data);
      m_size++;
      pthread_cond_signal(&m_cond_noempty);  //通知已经是非空的队列了

      return 0;
    }

    /* return value:
     *  -1  error
     *  0   没有返回，可能是超时等问题,需要重试
     *  1   返回出队的数据
     */
    int dequeue(T &data)               //非阻塞操作暴露给消费者,超时返回
    {
      ScopedLock lock(&m_list_mutex);
      timespec tsp;
      makeTimeoutSeconds(&tsp, m_seconds);

      if(0 == m_size)
      {
        int ret = pthread_cond_timedwait(&m_cond_noempty, &m_list_mutex, &tsp); //出队列的时候考虑是否为空
        if(0 != ret && ETIMEDOUT != ret) return -1;
      }

      if(!m_list.empty())
      {
        data = m_list.front();
        m_list.pop_front();
        m_size--;
        pthread_cond_signal(&m_cond_nofull);  //通知已经是非满的队列了
        return 1;
      }

      return 0;
    }

    int getSize(void)
    {
      ScopedLock lock(&m_list_mutex);
      return m_size;
    }

  private:
    void makeTimeoutSeconds(timespec *tsp, int seconds)
    {
      struct timeval now;
      gettimeofday(&now, NULL);
      tsp->tv_sec = now.tv_sec;
      tsp->tv_nsec = now.tv_usec * 1000;
      tsp->tv_sec = tsp->tv_sec + seconds;
    }

  private:

    std::list<T>      m_list;         //队列
    pthread_cond_t    m_cond_noempty; //条件：队列非空
    pthread_cond_t    m_cond_nofull;  //条件：队列非满
    pthread_mutex_t   m_list_mutex;   //队列锁

    int               m_seconds;      //消费者超时秒数          
    int               m_max_size;     //队列的最大size
    int               m_size;         //队列当前的size
};


#endif /* _BLOCKQUEUE_H_ */


