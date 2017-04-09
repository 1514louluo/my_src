#pragma once


#include "logdef.h"
#include "mycommon.h"
#include "SDUtility.h"
#include "logdef.h"
#include "block_queue.h"
#include "commondef.h"
#include "boost/format.hpp"
typedef struct _STU_SERVICE_STAT
{
	int service_id;
	int count;

}STU_SERVICE_STAT;

typedef struct _STU_SERVICE_INFO
{
	int service_id;
	uint64_t current_total_requst;
	uint64_t last_total_request;
	std::string cmd_path;

}STU_SERVICE_INFO;

typedef struct _STU_DB_STAT
{
	int rw_type;
	uint64_t delaytime_usec;
}STU_DB_STAT;

typedef struct _STU_REQ_RESP_STAT
{
	int cmd_type;
	uint64_t requesttime_millsec;
	uint64_t responsetime_millsec;
}STU_REQ_RESP_STAT;

#define  DB_RW_TYPE_READ   0
#define  DB_RW_TYPE_WRITE  1
#define  MONGO_RW_TYPE_READ   2
#define  MONGO_RW_TYPE_WRITE  3


typedef struct _STU_MEMCACHE_STAT
{
	int rw_type;
	int delay;
}STU_MEMCACHE_STAT;
#define  MEMCACHE_RW_TYPE_READ    0
#define  MEMCACHE_RW_TYPE_WRITE   1


class service_stat
{
public:
	static service_stat *singleton(void);
	void service_notify(int servicetypeid);
	void set_current_conn(int conn_count);
	void set_total_conn(uint64_t conn_count);
	void add_db_stat(int rw_type, int delay_millsec);
	void add_req_resp_stat(STU_REQ_RESP_STAT &mystat);
	void add_memcache_stat(STU_MEMCACHE_STAT &mystat);
	void add_loginbox_result_stat(bool loginresult);
	void add_logingame_result_stat(bool logingame_result);
	void add_logingame_duration_stat(uint64_t duration);
	// 当前正有效的连接数
	int m_current_conn_count;
	// 当前时刻总共的连接数
	uint64_t m_current_total_client;
	// 上一次统计时刻总共的连接数
	uint64_t m_last_total_client;
	// 最近一次60s时间段内统计的新连接数
	uint64_t m_60s_client_conn;

	// db
	uint64_t m_db_read_60s_count;
	uint64_t m_db_write_60s_count;
	uint64_t m_db_read_60s_delay;
	uint64_t m_db_write_60s_delay;

	// mongodb
	uint64_t m_mongo_read_60s_count;
	uint64_t m_mongo_write_60s_count;
	uint64_t m_mongo_read_60s_delay;
	uint64_t m_mongo_write_60s_delay;

	//machine status
	int m_cpu;
	float m_mem;
	int m_overload;

	// requst response stat
	int m_req_resp_count_60s;
	int m_req_resp_delay_millsec;

	// memcache stat
	uint64_t m_memcache_read_count_60s;
	uint64_t m_memcache_write_count_60s;
	uint64_t m_memcache_read_delay_60s;
	uint64_t m_memcache_write_delay_60s;

	// loginresult stat
	uint64_t m_loginresult_fail_rate;

	// logingame stat
	uint64_t m_loginbame_result_fail_rate;
	uint64_t m_logingame_duration_stat;

	std::string m_str_tmp_path;
	std::string m_str_local_ip;


	
private:
	service_stat(void);
	~service_stat(void);
	static service_stat *m_pthis;
	void init_stat_service(void);
	block_queue<STU_SERVICE_STAT> m_queue_service_stat;
	
	static void *service_stat_proc(void *param);
	void service_stat_doit(void);
	static void *machine_info_stat(void *param);
	//std::map< int,std::string> m_map_request_path;
	std::map<int, STU_SERVICE_INFO *> m_map_service;
	void set_service_info(int id, char *pservice_path);
	void update_service_stat(void);
	int get_cpu(int &cpu_usage);
	int get_mem(float &mem_usage);
	int get_overload(int &overload);

	//MyMutex m_mutex1;
	

	// db 统计
	std::list<STU_DB_STAT> m_list_db_stat;
	pthread_mutex_t        m_mutex_list_db_stat;

	// request--response统计
	std::list<STU_REQ_RESP_STAT> m_list_req_resp_stat;
	pthread_mutex_t        m_mutex_list_req_resp_stat;

	// MEMCACHE 统计
	std::list<STU_MEMCACHE_STAT> m_list_memcache_stat;
	MyMutex m_mutex_list_memcache_stat;

	// loginbox result stat
	std::list<bool>  m_list_loginbox_result_stat;
	MyMutex m_mutex_list_loginbox_result_stat;

	// logingame result stat
	std::list<bool> m_list_logingame_result_stat;
	MyMutex m_mutex_list_logingame_result_stat;
	
	// logingame duration stat
	std::list<uint64_t> m_list_logingame_duration_stat;
	MyMutex m_mutex_list_logingame_duration_stat;

	DECL_LOGGER(logger);
	

};
