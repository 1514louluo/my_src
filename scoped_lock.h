#ifndef _SCOPEDLOCK_H
#define _SCOPEDLOCK_H
#include <pthread.h>
//author: manialuo

#define SCOPED_LOCK_LOCKED   0
#define SCOPED_LOCK_UNLOCKED 1
class ScopedLock
{
public:

	ScopedLock(pthread_mutex_t *pmutex)
	{
		m_pmutex = pmutex;
		pthread_mutex_lock(m_pmutex);
		m_lock_tag = SCOPED_LOCK_LOCKED;
	}

	~ScopedLock()
	{
		if(m_lock_tag == SCOPED_LOCK_LOCKED)
		{
			pthread_mutex_unlock(m_pmutex);
		}
	}

	void unlock()
	{
		if(m_lock_tag == SCOPED_LOCK_LOCKED)
		{
			pthread_mutex_unlock(m_pmutex);
			m_lock_tag = SCOPED_LOCK_UNLOCKED;
		}
	}

private:
	int m_lock_tag;
	pthread_mutex_t *m_pmutex;
};

#endif  //_SCOPEDLOCK_H

