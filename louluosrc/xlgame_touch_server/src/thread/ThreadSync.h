#ifndef _SANDAI_THREADSYNC_H_20060407
#define _SANDAI_THREADSYNC_H_20060407

#include <pthread.h>
#include <vector>
#include <string>


inline int MUTEX_CHECK_INIT(pthread_mutex_t * p_mutex,const pthread_mutexattr_t * attr){
    return pthread_mutex_init(p_mutex,attr);
}
inline int MUTEX_CHECK_LOCK(pthread_mutex_t * p_mutex){
    return pthread_mutex_lock(p_mutex);
}
inline int MUTEX_CHECK_UNLOCK(pthread_mutex_t * p_mutex){
    return pthread_mutex_unlock(p_mutex);
}

class CMutex
{
public:
	CMutex();
	~CMutex();
	//0--failed, 1--success
	int Lock();
	//0--failed, 1--success
	int TryLock();
	//0--failed, 1--success
	int Unlock();
	
private:
	//0--failed, 1--success
	int Init();
	
private:
	pthread_mutex_t m_mutex;
};

class CRWLock
{
public:
	CRWLock();
	~CRWLock();
	//0--failed, 1--success
	int ReadLock();
	//0--failed, 1--success
	int WriteLock();
	//0--failed, 1--success
	int TryReadLock();
	//0--failed, 1--success
	int TryWriteLock();
	//0--failed, 1--success
	int ReadUnlock();
	//0--failed, 1--success
	int WriteUnlock();		
private:
	//0--failed, 1--success
	int Init();
private:
	pthread_rwlock_t m_rwlock;
};

class CScopeGuard
{
public:
	CScopeGuard(CMutex& mutex);
	~CScopeGuard();

	//0--failed, 1--success
	int Lock();
	//0--failed, 1--success
	int Unlock();
private:
	int 		m_bLocked;
	CMutex* m_pMutex;
};

class CRWScopeGuard
{
public:
	CRWScopeGuard(CRWLock& rwlock,bool is_readlock = true);
	~CRWScopeGuard();

	//0--failed, 1--success
	int WriteLock();
	//0--failed, 1--success
	int WriteUnlock();
	
	//0--failed, 1--success
	int ReadLock();
	//0--failed, 1--success
	int ReadUnlock();
private:
	int			m_bLocked;
	CRWLock*	m_prwLock;
};

class CCondition
{
public:
	CCondition();
	~CCondition();


	//0--failed, 1--success
	int LockCondition();
	//0--failed, 1--success
	int UnlockCondition();
	//0--failed, 1--success
	int NotifyCondition();
	//0--failed, 1--success
	int BroadcastCondition();
	//0--failed, 1--success
	int WaitCondition();
	//0--failed, 1--success, 2--timeout
	int TimedWaitCondition(unsigned int milli);
private:
	//0--failed, 1--success
	int Init();
private:
	pthread_mutex_t m_mutex;
	pthread_cond_t m_condition;		
};


#endif //_SANDAI_THREADSYNC_H_20060407

