#include "ThreadSync.h"
#include <sys/time.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
////////////////////////////////////////////////////////////////////////////////
//
//		  Class CMutex,底层互斥量封装
/******************************************************************************
函数名 :CMutex
功能   :构造函数,初始化类成员.
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CMutex::CMutex()
{
	memset(&m_mutex, 0, sizeof(pthread_mutex_t));
	Init();
}

/******************************************************************************
函数名 :~CMutex
功能   :释构函数,清除系统互斥量
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CMutex::~CMutex()
{
	pthread_mutex_unlock(&m_mutex);
	pthread_mutex_destroy(&m_mutex);
}

/******************************************************************************
函数名 :Init
功能   :初始化系统互斥量
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CMutex::Init()
{
	if (pthread_mutex_init(&m_mutex, NULL) != 0)
	{
		return 0;
	}
	
	return 1;
}

/******************************************************************************
函数名 :Lock
功能   :对系统互斥量进行加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CMutex::Lock()
{
	if (0 == pthread_mutex_lock(&m_mutex))
	{
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :TryLock
功能   :尝试对系统互斥量加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CMutex::TryLock()
{
	if (0 == pthread_mutex_trylock(&m_mutex))
	{
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :Unlock
功能   :对系统互斥量解锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CMutex::Unlock()
{
	if (0 == pthread_mutex_unlock(&m_mutex))
	{
		return 1;
	}
	
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
//
//		  Class CRWLock,底层读写锁封装
/******************************************************************************
函数名 :CRWLock
功能   :构造函数,初始化类成员.
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CRWLock::CRWLock()
{
	memset(&m_rwlock, 0, sizeof(pthread_rwlock_t));
	Init();
}

/******************************************************************************
函数名 :~CRWLock
功能   :释构函数,清除系统读写锁
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CRWLock::~CRWLock()
{	
	pthread_rwlock_destroy(&m_rwlock);
}

/******************************************************************************
函数名 :Init
功能   :初始化系统读写锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWLock::Init()
{	 
	if ( (pthread_rwlock_init(&m_rwlock, NULL) != 0))
	{
		return 0;
	}
	
	return 1;
}

/******************************************************************************
函数名 :ReadLock
功能   :对系统读写锁进行读加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWLock::ReadLock()
{
	if (0 == pthread_rwlock_rdlock(&m_rwlock))
	{		
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :WriteLock
功能   :对系统读写锁进行读加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWLock::WriteLock()
{
	if (0 == pthread_rwlock_wrlock(&m_rwlock))
	{
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :TryReadLock
功能   :尝试对系统读写锁进行读加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWLock::TryReadLock()
{
	if (0 == pthread_rwlock_tryrdlock(&m_rwlock))
	{		
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :TryWriteLock
功能   :尝试对系统读写锁进行写加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWLock::TryWriteLock()
{
	if (0 == pthread_rwlock_trywrlock(&m_rwlock))
	{
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :ReadUnlock
功能   :对系统读写锁进行读解锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWLock::ReadUnlock()
{
	if (0 == pthread_rwlock_unlock(&m_rwlock))
	{		
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :WriteUnlock
功能   :对系统读写锁进行写解锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWLock::WriteUnlock()
{
	if (0 == pthread_rwlock_unlock(&m_rwlock))
	{
		return 1;
	}
	
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
//
//		  Class CScopeGuard,简单的局部资源保护
/******************************************************************************
函数名 :CScopeGuard
功能   :构造函数,加系统锁
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CScopeGuard::CScopeGuard(CMutex& mutex)
{
	m_pMutex = &mutex;
	if (m_pMutex->Lock())
	{
		m_bLocked = 1;
	}
	else
	{
		m_bLocked = 0;
	}
}

/******************************************************************************
函数名 :~CScopeGuard
功能   :释构函数,解系统锁
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CScopeGuard::~CScopeGuard()
{
	if (m_bLocked)
	{
		if (m_pMutex->Unlock())
		{
			m_bLocked = 0;
		}
	}
}

/******************************************************************************
函数名 :Lock
功能   :对系统互斥量进行加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CScopeGuard::Lock()
{
	if (m_pMutex->Lock())
	{
		m_bLocked = 1;
		return 1;
	}
	else
	{
		m_bLocked = 0;
		return 0;
	}
}

/******************************************************************************
函数名 :Unlock
功能   :对系统互斥量解锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CScopeGuard::Unlock()
{
	if (m_bLocked)
	{
		if (m_pMutex->Unlock())
		{
			m_bLocked = 0;
			return 1;
		}
		return 0;
	}
	
	return 1;
}


////////////////////////////////////////////////////////////////////////////////
//
//		  Class CRWScopeGuard,简单的局部资源保护
/******************************************************************************
函数名 :CRWScopeGuard
功能   :构造函数,加系统锁
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CRWScopeGuard::CRWScopeGuard(CRWLock& rwlock,bool is_readlock)
{
	m_prwLock = &rwlock;

	if(is_readlock)
	{
		ReadLock();
	}
	else
	{
		WriteLock();
	}	
}

/******************************************************************************
函数名 :~CRWScopeGuard
功能   :释构函数,解系统锁
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CRWScopeGuard::~CRWScopeGuard()
{
	if (m_bLocked)
	{
		ReadUnlock();
	}
}

/******************************************************************************
函数名 :WriteLock
功能   :对系统读写锁的写锁加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWScopeGuard::WriteLock()
{
	if (m_prwLock->WriteLock())
	{
		m_bLocked = 1;
		return 1;
	}
	else
	{
		m_bLocked = 0;
		return 0;
	}
}

/******************************************************************************
函数名 :WriteUnlock
功能   :对系统读写锁的写锁解锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWScopeGuard::WriteUnlock()
{
	if (m_bLocked)
	{
		if (m_prwLock->WriteUnlock())
		{
			m_bLocked = 0;
			return 1;
		}
		return 0;
	}
	
	return 1;
}

/******************************************************************************
函数名 :ReadLock
功能   :对系统读写锁的读锁加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWScopeGuard::ReadLock()
{
	if (m_prwLock->ReadLock())
	{
		m_bLocked = 1;
		return 1;
	}
	else
	{
		m_bLocked = 0;
		return 0;
	}
}

/******************************************************************************
函数名 :ReadUnlock
功能   :对系统读写锁的读锁解锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CRWScopeGuard::ReadUnlock()
{
	if (m_bLocked)
	{
		if (m_prwLock->ReadUnlock())
		{
			m_bLocked = 0;
			return 1;
		}
		return 0;
	}
	
	return 1;
}






////////////////////////////////////////////////////////////////////////////////
//
//		  Class CCondition,底层条件通知封装
/******************************************************************************
函数名 :CCondition
功能   :构造函数,初始化类成员.
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CCondition::CCondition()
{
	memset(&m_condition, 0, sizeof(pthread_cond_t));
	memset(&m_mutex, 0, sizeof(pthread_mutex_t));
	Init();
}

/******************************************************************************
函数名 :~CCondition
功能   :释构函数,清除系统条件量
输入   :无
输出   :无
返回值 :无
其他   :无
******************************************************************************/
CCondition::~CCondition()
{
	pthread_cond_broadcast(&m_condition);
	pthread_cond_destroy(&m_condition);
	pthread_mutex_unlock(&m_mutex);
	pthread_mutex_destroy(&m_mutex);
}

/******************************************************************************
函数名 :Init
功能   :初始化系统条件量
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CCondition::Init()
{
	if ((pthread_mutex_init(&m_mutex, NULL) != 0) || (pthread_cond_init(&m_condition, NULL) != 0))
	{
		return 0;
	}
	return 1;
}

/******************************************************************************
函数名 :LockCondition
功能   :对系统条件量进行加锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CCondition::LockCondition()
{
	if (0 == pthread_mutex_lock(&m_mutex))
	{
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :UnlockCondition
功能   :对系统条件量解锁
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CCondition::UnlockCondition()
{
	if (0 == pthread_mutex_unlock(&m_mutex))
	{
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :NotifyCondition
功能   :触发系统条件量
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CCondition::NotifyCondition()
{
	if (0 == pthread_cond_signal(&m_condition))
	{
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :BroadcastCondition
功能   :广播条件发生.
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CCondition::BroadcastCondition()
{
	if (0 == pthread_cond_broadcast(&m_condition))
	{
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :WaitCondition
功能   :等待条件发生.
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CCondition::WaitCondition()
{
	if (0 == pthread_cond_wait(&m_condition, &m_mutex))
	{
		return 1;
	}
	
	return 0;
}

/******************************************************************************
函数名 :TimedWaitCondition
功能   :在一段时间内等待条件发生.
输入   :无
输出   :无
返回值 :成功返回1,失败0
其他   :无
******************************************************************************/
int CCondition::TimedWaitCondition(unsigned int milli)
{
	int iResult = 0;
	struct timespec abs_ts; 
	struct timeval cur_tv;
	if (-1 == gettimeofday(&cur_tv, NULL))
	{
		return 0;
	}
	
	int nsec = (cur_tv.tv_usec + milli % 1000) * 1000;
	if (nsec > 999999999)
	{
		abs_ts.tv_sec = cur_tv.tv_sec + milli / 1000 + 1;
		abs_ts.tv_nsec = nsec - 999999999;
	}
	else
	{
		abs_ts.tv_sec = cur_tv.tv_sec + milli / 1000;
		abs_ts.tv_nsec = nsec;
	}
		
	iResult = pthread_cond_timedwait(&m_condition, &m_mutex, &abs_ts);
	if (iResult != 0)
	{
		switch (iResult)
		{
			case ETIMEDOUT:
			{
				timeval timeWait;
				timeWait.tv_sec = 0;
				timeWait.tv_usec = 2000;
				select(0, (fd_set*)0, (fd_set*)0, (fd_set*)0, &timeWait);
				return 2;
			}
			default:
			{
				return 0;
			}
		}
	}
	
	return 1;
}

