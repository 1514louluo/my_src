#include "service_stat.h"


IMPL_LOGGER(service_stat, logger)

service_stat *service_stat::m_pthis=NULL;


service_stat::service_stat(void)
{
	m_current_conn_count = 0;
	m_current_total_client = 0;
	m_last_total_client = 0;
	m_60s_client_conn = 0;

	m_db_read_60s_count=0;
	m_db_write_60s_count=0;
	m_db_read_60s_delay=0;
	m_db_write_60s_delay=0;

	m_mongo_read_60s_count=0;
	m_mongo_write_60s_count=0;
	m_mongo_read_60s_delay=0;
	m_mongo_write_60s_delay=0;


	m_req_resp_count_60s=0;
	m_req_resp_delay_millsec=0;

	m_memcache_read_count_60s=0;
	m_memcache_write_count_60s=0;
	m_memcache_read_delay_60s=0;
	m_memcache_write_delay_60s=0;

	m_str_tmp_path = "/tmp/store_tmp";
	m_str_local_ip = SDUtility::ip_search();
	pthread_mutex_init(&m_mutex_list_db_stat, NULL);
	pthread_mutex_init(&m_mutex_list_req_resp_stat, NULL);

	m_loginresult_fail_rate=0;

}
 
service_stat::~service_stat(void)
{
}

service_stat * service_stat::singleton( void )
{

	if (m_pthis == NULL)
	{
		m_pthis = new service_stat;
		m_pthis->init_stat_service();
	}
	return m_pthis;
}


 

void service_stat::service_notify( int servicetypeid )
{

	/*STU_SERVICE_STAT stat;
	stat.service_id = servicetypeid;
	stat.count = 1;
	m_queue_service_stat.add_queue(stat);*/
}

void * service_stat::service_stat_proc( void *param )
{
	if (param == NULL)
	{
		return (void *)0;
	}

	service_stat *pthis = (service_stat *)param;
	pthis->service_stat_doit();
	
	SYS_LOG_ERROR("service_stat::service_stat_proc terminated");
	return (void *)0;

}

void * service_stat::machine_info_stat( void *param )
{

	if (param == NULL)
	{
		return (void *)0;
	}

	service_stat *pthis = (service_stat *)param;
	while (true)
	{
		sleep(10);
		pthis->get_cpu(pthis->m_cpu);
		pthis->get_mem(pthis->m_mem);
		pthis->get_overload(pthis->m_overload);
	}

}


void service_stat::service_stat_doit( void )
{

	SYS_LOG_INFO("service_stat::service_stat_proc start");
	int result;
	STU_SERVICE_STAT stat;
	time_t lasttime=time(NULL);
	while (true)
	{
		result = m_queue_service_stat.dequeue_expand(stat);
		if (result != 1)
		{
			time_t currenttime = time(NULL);
			if (currenttime - lasttime > 60)
			{
				lasttime = currenttime;
				update_service_stat();

			}
			continue;
		}

		std::map< int,STU_SERVICE_INFO *>::iterator iter;
		iter = m_map_service.find(stat.service_id);
		if (iter == m_map_service.end())
		{
			SYS_LOG_ERROR("service stat, unsupported typeid, id="<<stat.service_id);
			continue;
		}
		STU_SERVICE_INFO *stat_info = iter->second;
		
		if (stat_info == NULL)
		{
			SYS_LOG_ERROR("stat_info == NULL");
			continue;
		}

		stat_info->current_total_requst++;
		
		time_t currenttime = time(NULL);
		if (currenttime - lasttime > 60)
		{
			lasttime = currenttime;
			update_service_stat();

		}

		

	}

}




int service_stat::get_cpu(int &cpu_usage)
{
	std::string str_random;
	str_random = (boost::format("%d") %random()).str();
	std::string str_file_path;
	str_file_path = m_str_tmp_path+str_random;
	pid_t thispid;
	thispid = getpid();
	std::string cmdline;
	cmdline = ((boost::format("rm -f %s")  %str_file_path.c_str())).str();
	::system(cmdline.c_str());
	cmdline = ((boost::format("top -bn 1 -p %d | grep %d | awk '{print $9}' > %s") %thispid  %thispid %str_file_path.c_str())).str();
	::system(cmdline.c_str());

	//int fd = ::open(m_str_cpu_path.c_str(), O_CREAT);
	std::ifstream in(str_file_path.c_str());
	std::string str_value;
	//std::getline(fin, line)
	std::getline(in,str_value);
	double fvalue = atof(str_value.c_str());
	cpu_usage = fvalue;
	in.close();

	//printf("cpu_usage = %d\n",cpu_usage);

	cmdline = (boost::format("rm -f %s")  %str_file_path.c_str()).str();
	::system(cmdline.c_str());
	return 0;

}


int service_stat::get_mem(float &mem_usage)
{

	std::string str_random;
	str_random = (boost::format("%d") %random()).str();
	std::string str_file_path;
	str_file_path = m_str_tmp_path+str_random;

	pid_t thispid;
	thispid = getpid();
	std::string cmdline;
	cmdline = ((boost::format("rm -f %s")  %str_file_path.c_str())).str();
	::system(cmdline.c_str());
	cmdline = ((boost::format("top -bn 1 -p %d | grep %d | awk '{print $10}' > %s") %thispid  %thispid %str_file_path.c_str())).str();
	::system(cmdline.c_str());

	//int fd = ::open(m_str_cpu_path.c_str(), O_CREAT);
	std::ifstream in(str_file_path.c_str());
	std::string str_value;
	std::getline(in, str_value);
	double fvalue = atof(str_value.c_str());
	mem_usage = fvalue;
	in.close();

	//printf("mem_usage = %d\n",mem_usage);

	cmdline = (boost::format("rm -f %s")  %str_file_path.c_str()).str();
	::system(cmdline.c_str());
	return 0;

}


int service_stat::get_overload(int &overload)
{

	std::string str_random;
	str_random = (boost::format("%d") %random()).str();
	std::string str_file_path;
	str_file_path = m_str_tmp_path+str_random;

	pid_t thispid;
	thispid = getpid();
	std::string cmdline;
	cmdline = ((boost::format("rm -f %s")  %str_file_path.c_str())).str();
	::system(cmdline.c_str());
	cmdline = ((boost::format("top -bn 1 |grep \"load average\"| cut -d \",\" -f 4| awk '{print $3}' > %s")  %str_file_path.c_str()) ).str();
	::system(cmdline.c_str());

	//int fd = ::open(m_str_cpu_path.c_str(), O_CREAT);
	std::ifstream in(str_file_path.c_str());
	std::string str_value;
	std::getline(in,str_value);
	double fvalue = atof(str_value.c_str());
	overload = fvalue;
	in.close();

	//printf("overload = %d\n",overload);

	cmdline = (boost::format("rm -f %s")  %str_file_path.c_str()).str();
	::system(cmdline.c_str());
	return 0;

}

void service_stat::init_stat_service( void )
{

	// 登陆注册协议集
	set_service_info(GAMEBOX_SERVER_PROPERTY_POST_ID, GAMEBOX_SERVER_PROPERTY_POST);
	set_service_info(GAMEBOX_SERVER_PROPERTY_DELETE_ID, GAMEBOX_SERVER_PROPERTY_DELETE);
	set_service_info(GAMEBOX_SERVER_PROPERTY_QUERY_ID, GAMEBOX_SERVER_PROPERTY_QUERY);
	set_service_info(GAMEBOX_SERVER_PROPERTY_QUERYBATCH_ID, GAMEBOX_SERVER_PROPERTY_QUERYBATCH);

	int result;
	pthread_t threadid;
	result = pthread_create(&threadid, NULL, service_stat::service_stat_proc, this);
	if (result == -1)
	{
		SYS_LOG_ERROR("failed to start thread service_stat::service_stat_proc");
		return;
	}

	result = pthread_create(&threadid, NULL, service_stat::machine_info_stat, this);
	if (result == -1)
	{
		SYS_LOG_ERROR("failed to start thread service_stat::machine_info_stat");
		return;
	}

}

void service_stat::set_service_info( int id, char *pservice_path)
{
	STU_SERVICE_INFO *pservice;
	pservice = new STU_SERVICE_INFO;
	
	pservice->service_id= id;
	pservice->current_total_requst = 0;
	pservice->last_total_request = 0;
	pservice->cmd_path = pservice_path;
	m_map_service.insert(std::make_pair(id, pservice));
}

void service_stat::update_service_stat( void )
{

	std::map<int, STU_SERVICE_INFO *>::iterator iter;
	for (iter=m_map_service.begin(); iter != m_map_service.end(); iter++)
	{
		STU_SERVICE_INFO *pinfo = iter->second;
		uint64_t diff = pinfo->current_total_requst - pinfo->last_total_request;
		pinfo->last_total_request = pinfo->current_total_requst;
		if (diff == 0)
		{
			continue;
		}
		STAT_LOG_INFO("type="<<iter->first<<" path="<<pinfo->cmd_path<<" 60s_req="<<diff<<" current_total_request="
				<<pinfo->current_total_requst);
		
	}

	// deal with connection
	uint64_t diff_conn = m_current_total_client - m_last_total_client;
	m_60s_client_conn = diff_conn;
	m_last_total_client = m_current_total_client;
	STAT_LOG_INFO("current_client_connection="<<m_current_conn_count<<" 60s_conn="<<diff_conn<<
		" connection_1s="<<(int)((double)diff_conn/60));

	//-------------------------- deal with db rw------------------------------------
	{
		scopedlock mylock(&m_mutex_list_db_stat);
		uint32_t read_count=0;
		uint64_t read_delay_calculate=0;
		uint32_t write_count=0;
		uint64_t write_delay_calculate=0;
		//----mongodb
		uint32_t mongodb_read_count=0;
		uint64_t mongodb_read_delay_calculate=0;
		uint32_t mongodb_write_count=0;
		uint64_t mongodb_write_delay_calculate=0;

		while( ! m_list_db_stat.empty())
		{
			STU_DB_STAT stat;
			stat = m_list_db_stat.front();
			m_list_db_stat.pop_front();
			if (stat.rw_type == DB_RW_TYPE_READ)
			{
				read_count++;
				read_delay_calculate = read_delay_calculate + stat.delaytime_usec;
			} 
			else if(stat.rw_type == DB_RW_TYPE_WRITE)
			{
				write_count++;
				write_delay_calculate = write_delay_calculate + stat.delaytime_usec;
			}
			else if (stat.rw_type == MONGO_RW_TYPE_READ)
			{
				mongodb_read_count++;
				mongodb_read_delay_calculate = mongodb_read_delay_calculate+stat.delaytime_usec;
			}
			else if (stat.rw_type == MONGO_RW_TYPE_WRITE)
			{
				mongodb_write_count++;
				mongodb_write_delay_calculate = mongodb_write_delay_calculate+stat.delaytime_usec;
			}
		}

		m_db_read_60s_count = read_count;
		m_db_write_60s_count = write_count;
		m_mongo_read_60s_count = mongodb_read_count;
		m_mongo_write_60s_count = mongodb_write_count;

		if (read_count != 0)
		{
			m_db_read_60s_delay = (uint64_t)(((double)read_delay_calculate)/((double)read_count));
		} 
		
		if (write_count != 0)
		{
			m_db_write_60s_delay = (uint64_t)(((double)write_delay_calculate)/((double)write_count));
		}
		if (mongodb_read_count != 0)
		{
			m_mongo_read_60s_delay = (uint64_t)(((double)mongodb_read_delay_calculate)/((double)mongodb_read_count));
		}
		if (mongodb_write_count != 0)
		{
			m_mongo_write_60s_delay = (uint64_t)(((double)mongodb_write_delay_calculate)/((double)mongodb_write_count));
		}
		
	}
	STAT_LOG_INFO("60s_db_write_count="<<m_db_write_60s_count<<" average_delay_millsec="<<m_db_write_60s_delay
		<<" 60s_db_read_count="<<m_db_read_60s_count<<" average_delay_millsec="<<m_db_read_60s_delay);

	//-----------------------deal with request response------------------------------
	{
		// 注：这个地方没有按照协议类型来分，后续将细化，根据协议类型来区分
		scopedlock mylock2(&m_mutex_list_req_resp_stat);
		uint64_t req_resp_count = 0;
		uint64_t req_resp_delays = 0;
		while (!m_list_req_resp_stat.empty())
		{
			STU_REQ_RESP_STAT req_resp_stat;
			req_resp_stat = m_list_req_resp_stat.front();
			m_list_req_resp_stat.pop_front();
			req_resp_count++;
			req_resp_delays = req_resp_delays + (req_resp_stat.responsetime_millsec-req_resp_stat.requesttime_millsec);
			//STAT_LOG_INFO("req_resp_delays="<<req_resp_delays);
		}
		m_req_resp_count_60s = req_resp_count;
		if (req_resp_count != 0)
		{
			m_req_resp_delay_millsec = (int)((double)req_resp_delays / (double) req_resp_count);
		}
		else
		{

		}
		

	}
	STAT_LOG_INFO("60s_request_count="<<m_req_resp_count_60s<<" 60s_req_resp_average_delay_millsec="<<m_req_resp_delay_millsec);

	// ----------------------deal with memcache stat----------------
	uint64_t memcache_read_count = 0;
	uint64_t memcache_write_count=0;
	uint64_t memcache_read_delay=0;
	uint64_t memcache_write_delay=0;
	{
		scopedlock mylock3(m_mutex_list_memcache_stat.get());
		
		while (!m_list_memcache_stat.empty())
		{
			STU_MEMCACHE_STAT stat;
			stat = m_list_memcache_stat.front();
			if (stat.rw_type == MEMCACHE_RW_TYPE_READ)
			{
				memcache_read_count++;
				memcache_read_delay = memcache_read_delay+ stat.delay;
			} 
			else
			{
				memcache_write_count++;
				memcache_write_delay = memcache_write_delay+ stat.delay;
			}
			m_list_memcache_stat.pop_front();
		}
	}
	if (memcache_read_count==0)
	{
		m_memcache_read_count_60s = 0;
		m_memcache_read_delay_60s = 0;
	}
	else
	{
		m_memcache_read_count_60s = memcache_read_count;
		m_memcache_read_delay_60s = memcache_read_delay/memcache_read_count;
	}
	if (memcache_write_count == 0)
	{
		m_memcache_write_count_60s = 0;
		m_memcache_write_count_60s = 0;
	} 
	else
	{
		m_memcache_write_count_60s = memcache_write_count;
		m_memcache_write_delay_60s = memcache_write_delay/memcache_write_count;
	}
	SYS_LOG_DEBUG("60s memcache read count: "<<m_memcache_read_count_60s<<" read delay: "<<m_memcache_read_delay_60s);
	SYS_LOG_DEBUG("60s memcache write count :"<<m_memcache_write_count_60s<<" write delay: "<<m_memcache_write_delay_60s);
	
	// --------- dealwith loginbox result stat---------------------------------------
	uint64_t loginresult_ok_count=0;
	uint64_t loginresult_err_count=0;
	{
		scopedlock mylock4(m_mutex_list_loginbox_result_stat.get());
		while (!m_list_loginbox_result_stat.empty())
		{
			bool loginresult = m_list_loginbox_result_stat.front();
			if (loginresult == true)
			{
				loginresult_ok_count++;
			} 
			else
			{
				loginresult_err_count++;
			}
			
			m_list_loginbox_result_stat.pop_front();
		}

	}
	if (loginresult_err_count+loginresult_ok_count == 0)
	{
		m_loginresult_fail_rate=0;
	} 
	else
	{
		m_loginresult_fail_rate = 
			(uint64_t)(((double)loginresult_err_count)/( (double)(loginresult_err_count+loginresult_ok_count) )*100);

	}

	// -------------deal with logingame result stat -------------
	uint64_t logingame_result_ok_count=0;
	uint64_t logingame_result_err_count=0;
	{
		scopedlock mylock5(m_mutex_list_logingame_result_stat.get());
		while (!m_list_logingame_result_stat.empty())
		{
			bool logingame_result = m_list_logingame_result_stat.front();
			m_list_logingame_result_stat.pop_front();
			if (logingame_result==true)
			{
				logingame_result_ok_count++;
			} 
			else
			{
				logingame_result_err_count++;
			}
		}
	}
	if (logingame_result_ok_count+logingame_result_err_count==0)
	{
		m_loginbame_result_fail_rate=0;
	} 
	else
	{
		m_loginbame_result_fail_rate = (uint64_t)(((double)logingame_result_err_count)/( (double)(logingame_result_err_count+logingame_result_ok_count) )*100);
	}

	//--------------   deal with logingame duration stat ------------
	uint64_t duration_logingame_sum=0;
	uint64_t duration_logingame_count=0;
	{
		scopedlock mylock6(m_mutex_list_logingame_duration_stat.get());
		while (!m_list_logingame_duration_stat.empty())
		{
			uint64_t duration = m_list_logingame_duration_stat.front();
			m_list_logingame_duration_stat.pop_front();
			duration_logingame_sum = duration_logingame_sum+duration;
			duration_logingame_count++;
		}
	}
	if (duration_logingame_count==0)
	{
		m_logingame_duration_stat=0;
	} 
	else
	{
		m_logingame_duration_stat = (uint64_t)(((double)duration_logingame_sum)/((double)duration_logingame_count));
	}

	

	

}

void service_stat::set_current_conn( int conn_count )
{
	m_current_conn_count = conn_count;

}

void service_stat::set_total_conn( uint64_t conn_count )
{

	m_current_total_client = conn_count;
}

void service_stat::add_db_stat( int rw_type, int delay_millsec )
{

	STU_DB_STAT stat;
	stat.rw_type = rw_type;
	stat.delaytime_usec = delay_millsec;
	scopedlock lock(&m_mutex_list_db_stat);
	m_list_db_stat.push_back(stat);
}

void service_stat::add_req_resp_stat( STU_REQ_RESP_STAT &mystat )
{
	scopedlock mylock(&m_mutex_list_req_resp_stat);
	m_list_req_resp_stat.push_back(mystat);
}

void service_stat::add_memcache_stat( STU_MEMCACHE_STAT &mystat )
{
	scopedlock mylock(m_mutex_list_memcache_stat.get());
	m_list_memcache_stat.push_back(mystat);

}

void service_stat::add_loginbox_result_stat( bool loginresult )
{
	scopedlock mylock(m_mutex_list_loginbox_result_stat.get());
	m_list_loginbox_result_stat.push_back(loginresult);

}

void service_stat::add_logingame_result_stat( bool logingame_result )
{
	scopedlock mylock(m_mutex_list_logingame_result_stat.get());
	m_list_logingame_result_stat.push_back(logingame_result);

}

void service_stat::add_logingame_duration_stat( uint64_t duration )
{
	scopedlock mylock(m_mutex_list_logingame_duration_stat.get());
	m_list_logingame_duration_stat.push_back(duration);

}




