#include "threadpoll_manager.h"

#include "SDConfigurationSingleton.h"


IMPL_LOGGER(threadpoll_manager, logger);



threadpoll_manager::threadpoll_manager(void)
{
}

threadpoll_manager::~threadpoll_manager(void)
{
}

void threadpoll_manager::commit_task( boost::function<void(void)> task_proc )
{

	m_queue_task.add_queue(task_proc);
}

void * threadpoll_manager::general_thread_proc( void *param )
{
	if (param == NULL)
	{
		return (void *)0;
	}

	threadpoll_manager *pthis = (threadpoll_manager *)param;
	pthis->general_thread_doit();
	MY_LOGINFO("thread threadpoll_manager::general_thread_proc terminated");
	return (void *)0;

}

void threadpoll_manager::general_thread_doit( void )
{

	
	while (true)
	{
		int result;
		boost::function<void(void)> task;
		result = m_queue_task.dequeue_expand(task);
		if (result != 1)
		{
			continue;
		}
		//MY_LOGINFO("fetch one task,ready to exec task");
		if (task != NULL)
		{
			task();
		}
		
	}

}

void threadpoll_manager::init_thread_poll( void )
{


	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	int thread_count = config.getInt("threadpoll_thread_count",10);
	for (int i=0;i<thread_count; i++)
	{
		pthread_t threadid;
		int result = pthread_create(&threadid, NULL, threadpoll_manager::general_thread_proc, this);
		if (result == -1)
		{
			MY_LOGINFO("failed to create thread threadpoll_manager::general_thread_proc, exit");
			sleep(1);
			exit(0);
		}
	}

}


