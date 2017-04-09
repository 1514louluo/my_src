#pragma once

#include "mycommon.h"
#include "SDLogger.h"
class xltimer
{
public:
	xltimer(void);
	~xltimer(void);
	void startimer(boost::function<void(void)> timer_callback, uint32_t interval_seconds);

	void stoptimer(void);

private:
	DECL_LOGGER(logger);
	static void *timer_proc(void *param);
	void timer_doit(void);
	bool m_is_started;
	boost::function<void (void) > m_timer_callback;
	int m_interval;

};
