#pragma once

#include "boost/function.hpp"
#include "boost/bind.hpp"


#include "mycommon.h"

#include "block_queue.h"

#include "SDLogger.h"

class threadpoll_manager
{
public:
	threadpoll_manager(void);
	~threadpoll_manager(void);

	void commit_task(boost::function<void(void)>  task_proc);

	void init_thread_poll(void);

	
	



private:

	block_queue< boost::function<void(void)> > m_queue_task;

	static void *general_thread_proc(void *param);
	void general_thread_doit(void);
	//void *m_pparent;

	
	DECL_LOGGER(logger);
};
