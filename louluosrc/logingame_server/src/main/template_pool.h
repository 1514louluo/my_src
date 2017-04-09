#pragma once

#include "block_queue.h"
#include "SDLogger.h"
#include "SDUtility.h"
#include "global_var.h"
#include "mycommon.h"
template<typename T>
class template_pool
{
public:
	template_pool(void){};
	~template_pool(void){};
	void install_obj(T *pobj)
	{
		m_queue.add_queue(pobj);
		//printf("install obj, current obj=%d\n", m_queue.getsize());
	};
	int fetch_obj(T **ppobj)
	{
		uint32_t time_before = SDUtility::get_cur_millseconds();
		uint32_t time_after=0;
		int result;
		int retry_count = 0;
		while (true)
		{
			retry_count++;
			if (retry_count > 5)
			{
		//		MY_LOGERROR("fatal fetch mysql from pool timeout");
		//		PERFORM_LOG("mysql db pool fatal,failed to fetch 5 times");
				*ppobj = NULL;
				return -1;
			}
			T *pojb = NULL;
			result = m_queue.dequeue_expand(pojb);
			if (result != 1)
			{
				continue;
			}

			//time_after = SDUtility::get_cur_millseconds();
			//if (time_after-time_before>m_pool_fetch_timeout_threashold)
			{
			//	PERFORM_LOG("mysql db pool fetch timeout warning, delay millsec="<<time_after-time_before);
			}
		//	//MY_LOGDEBUG("mysql db pool fetch millsec="<<time_after-time_before);

		//	int avaliable = m_queue_mysql.getsize();
		//	if (avaliable < m_pool_low_water)
		//	{
		//		PERFORM_LOG("mysql obj lower  than threashold, avaliable="<<avaliable);
		//	}

			*ppobj = pojb;
			return 0;
		}
	};

	int restore_obj(T *pobj)
	{
		m_queue.add_queue(pobj);
		return 0;
	};


	//typedef T  objtype;
private:
	block_queue<T*> m_queue;
	DECL_LOGGER(logger);


	
};
