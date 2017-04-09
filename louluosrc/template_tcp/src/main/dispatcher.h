#pragma once


#include "tcpservermanager.h"
#include "SDLogger.h"
#include "adapter_common.h"
#include "mycommon.h"
#include "block_queue.h"
#include "message_codec.h"
#include "threadpoll_manager.h"
#include "redis_client.h"
#include "template_guard.h"
#include "template_pool.h"
#include "memcache_client.h"
#include "cJSON.h"
#include "protocol_codec.h"

#include "router_msg_service.h"
#include "zmq_client_pub.h"
#include "zmq_client_sub.h"
#include "xltimer.h"

#include "service_stat.h"

#include "tokenid_service.h"

class dispatcher
{
public:
	dispatcher(void);
	~dispatcher(void);

	void init(void);

	block_queue< boost::shared_ptr<STU_MSG_INFO> > m_queue_adapter_cmd;


private:
	DECL_LOGGER(logger);
	threadpoll_manager m_thread_pool;
	tcpservermanager m_tcpserver_man;
	template_pool<redis_client> m_redis_pool;
	template_pool<memcache_client> m_memcache_pool;

	zmq_client_pub m_zmq_client_pub;
	zmq_client_sub m_zmq_client_sub;
	void zmq_sub_recv_callback(std::string &str_data);
	void zmq_pub_recv_callback(std::string &str_data);
	//std::string m_zmq_router_notify_server_identity;   // 通知服务的identity
	//std::string m_zmq_router_app_server_identity;
	std::string m_zmq_service_self_identity;           // 服务自己的identity
	static void *adapter_cmd_proc(void *param);
	void adapter_cmd_doit(void);

	int set_randomkey_cache(std::string key);
	int remove_randomkey_cache(std::string key);
	int query_randomkey_cache(std::string key);

	int get_session_info(std::string &str_sessionid,STU_SESSION_INFO *psessioninfo);
	int json_to_session( std::string &str_json, STU_SESSION_INFO *psessioninfo );

	void process_auth(boost::shared_ptr<STU_MSG_INFO> prequest);
	void process_cli_query_trustee(boost::shared_ptr<STU_MSG_INFO> prequest);
	void process_getkey(boost::shared_ptr<STU_MSG_INFO> prequest);

	std::string m_zmq_router_app_server_identity;  // appserver的identity
	
	
};
