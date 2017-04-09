#pragma once

#include "mycommon.h"
#include "xltimer.h"
#include "SDUtility.h"
class service_stat
{
public:
	static service_stat *singleton(void);

	uint32_t m_tcp_curent_conn;
	uint64_t m_tcp_history_conn;

	int m_cpu_usage;
	float m_mem_usage;
	int m_overload;
	int m_cmd_counts_60seconds_stat;

	void add_cmd_count(void);

private:

	std::string m_str_tmp_path;
	
	int m_cmd_counts_60seconds;
	MyMutex m_mutex_cmd_count;
	void init_stat_service(void);
	xltimer m_timer_stat;
	void timer_callback(void);
	
	service_stat(void);
	~service_stat(void);
	static service_stat *m_pthis;


	int get_cpu(int &cpu_usage);
	int get_mem(float &mem_usage);
	int get_overload(int &overload);


	
};
