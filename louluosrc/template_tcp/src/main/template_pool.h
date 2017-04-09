#pragma once

#include "block_queue.h"
#include "SDLogger.h"
#include "SDUtility.h"
#include "global_var.h"
#include "mycommon.h"
#include "SDConfigurationSingleton.h"

template<typename T>
class template_pool
{
public:
	template_pool(void){
		const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
		m_objcount=0;
		m_lowlevel =config.getInt("template_pool_low_water", 50);
		m_pool_fetch_timeout_threashold=config.getInt("template_fetch_timeout_threashold",1000);


	};
	~template_pool(void){};
	void setpoolname(std::string name){m_pool_name = name;}
	void install_obj(T *pobj)
	{
		m_queue.add_queue(pobj);
		m_objcount++;
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
				PERFORM_LOG(""<<m_pool_name<<",failed to fetch 5 times");
				*ppobj = NULL;
				return -1;
			}
			T *pojb = NULL;
			result = m_queue.dequeue_expand(pojb);
			if (result != 1)
			{
				continue;
			}

			time_after = SDUtility::get_cur_millseconds();
			if (time_after-time_before>m_pool_fetch_timeout_threashold)
			{
				PERFORM_LOG(""<<m_pool_name<<" fetch timeout warning, delay millsec="<<time_after-time_before);
			}
			//MY_LOGDEBUG("mysql db pool fetch millsec="<<time_after-time_before);

			int avaliable = m_queue.getsize();
			int pool_low_water = (int)(((float)m_objcount)*( (double)m_lowlevel / 100.0));
			if (avaliable < pool_low_water)
			{
				PERFORM_LOG(""<<m_pool_name<<" lower  than threashold, avaliable="<<avaliable);
			}

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
	std::string m_pool_name;
	int m_objcount;
	int m_lowlevel;
	int m_pool_fetch_timeout_threashold;


	
};
