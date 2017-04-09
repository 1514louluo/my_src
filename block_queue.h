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


    int addqueue(T &data)              //ֻ�ṩ����������¶��������
    {
      ScopedLock lock(&m_list_mutex);  //����ʱ���ȿ����Ƿ��Ѿ�����
      while(m_max_size <= m_size)
        pthread_cond_wait(&m_cond_nofull, &m_list_mutex); 

      m_list.push_back(data);
      m_size++;
      pthread_cond_signal(&m_cond_noempty);  //֪ͨ�Ѿ��ǷǿյĶ�����

      return 0;
    }

    /* return value:
     *  -1  error
     *  0   û�з��أ������ǳ�ʱ������,��Ҫ����
     *  1   ���س��ӵ�����
     */
    int dequeue(T &data)               //������������¶��������,��ʱ����
    {
      ScopedLock lock(&m_list_mutex);
      timespec tsp;
      makeTimeoutSeconds(&tsp, m_seconds);

      if(0 == m_size)
      {
        int ret = pthread_cond_timedwait(&m_cond_noempty, &m_list_mutex, &tsp); //�����е�ʱ�����Ƿ�Ϊ��
        if(0 != ret && ETIMEDOUT != ret) return -1;
      }

      if(!m_list.empty())
      {
        data = m_list.front();
        m_list.pop_front();
        m_size--;
        pthread_cond_signal(&m_cond_nofull);  //֪ͨ�Ѿ��Ƿ����Ķ�����
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

    std::list<T>      m_list;         //����
    pthread_cond_t    m_cond_noempty; //���������зǿ�
    pthread_cond_t    m_cond_nofull;  //���������з���
    pthread_mutex_t   m_list_mutex;   //������

    int               m_seconds;      //�����߳�ʱ����          
    int               m_max_size;     //���е����size
    int               m_size;         //���е�ǰ��size
};


#endif /* _BLOCKQUEUE_H_ */


