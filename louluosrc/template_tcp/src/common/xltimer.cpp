#include "xltimer.h"

IMPL_LOGGER(xltimer, logger);

xltimer::xltimer(void)
{
	m_is_started = false;
	m_timer_callback = NULL;
	m_interval=100;
}

xltimer::~xltimer(void)
{
}

void xltimer::startimer( boost::function<void(void)> timer_callback, uint32_t interval_seconds )
{
	
	if (m_is_started == true)
	{
		return;
	}
	m_is_started=true;
	m_interval = interval_seconds;
	if (timer_callback == NULL)
	{
		MY_LOGERROR("xltimer::startimer ERROR, timer_callback==NULL");
		return;
	}
	if (interval_seconds == 0)
	{
		return;
	}
	m_timer_callback = timer_callback;
	int result;
	pthread_t threadid;
	result = pthread_create(&threadid, NULL, xltimer::timer_proc, this);
	if (result == -1)
	{
		MY_LOGERROR("failed to start thread xltimer::timer_proc");
		return;
	}

	MY_LOGDEBUG("xltimer::startimer  finished");
	


}

void * xltimer::timer_proc( void *param )
{
	if (param == NULL)
	{
		return (void *)param;

	}
	xltimer *pthis = (xltimer *)param;
	pthis->timer_doit();
	return (void *)NULL;

}

void xltimer::timer_doit( void )
{
	MY_LOGDEBUG("xltimer::timer_doit started");
	while (true)
	{
		sleep(m_interval);
		if (m_timer_callback != NULL)
		{
			m_timer_callback();
		}
	}


}
