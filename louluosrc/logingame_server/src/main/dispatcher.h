#pragma once

#include "block_queue.h"
#include "mycommon.h"
#include "HttpRequestPacket.h"
#include "threadpoll_manager.h"
#include "http_manager.h"
#include <sys/types.h>
#include "databasepool.h"
#include "memcache_cli_pool.h"
#include "libcurlclient.h"
#include "commondef.h"
//#include "verify_service.h"
#include "memcache_guard.h"
#include "trustee_service.h"
//#include "message01_service.h"
//#include "webjumpkey_service.h"
#include "global_var.h"
//#include "user_service.h"
//#include "tourist_service.h"
//#include "sessionid_man.h"
//#include "heartbeat_service.h"
//#include "login_realtime_service.h"
#include "loginbox_his_service.h"
#include "prevent_service.h"
#include "redis_pool.h"
#include "redis_cli_guard.h"
//#include "gamelib_service.h"
//#include "ipserver.h"
#include "json_codec.h"
#include "loginwebgame_service.h"
//#include "logingame_db_service.h"
//#include "systemmessage_service.h"
//#include "messageaddress_service.h"
//#include "systemtouserreply_service.h"
//#include "messagerecord_service.h"
//#include "message_feedback_service.h"
//#include "gameviewer_service.h"
//#include "bindtourist_service.h"
//#include "gameidfilter_service.h"
//#include "message_filter.h"
#include "mysql_config_loader.h"
#include "login_track_service.h"

#include "template_guard.h"
#include "template_guard.h"
#include "mongo_client.h"
#include "logingame_mongodb_service.h"
#include "cjson_wrapper.h"
#include "zmq_client_pub.h"
#include "master_slave_conf.h"
#include "gameinfo.h"
#include "general_log.h"
//#define  VALID_PARAM(param)   if(param.empty()) { box_client_response_error()

#define  MASTER_SLAVE_CHECK  if(master_slave_check() == -1){MY_LOGERROR("slave server, disable "<<request->m_str_cmdpath);box_client_response_error(SYSTEM_ERROR, request);return;}


class dispatcher
{
public:
	dispatcher(void);
	~dispatcher(void);

	void init(void);

	block_queue< boost::shared_ptr<HttpRequestPacket>  > m_request_queue;

	

	

	int box_client_response(HttpResponseHeaderPacket *phttp_response, int clientfd, 
							uint64_t client_unify_tag);

	int box_client_response_ok(boost::shared_ptr<HttpRequestPacket> prequest, general_log &log);
	//int box_client_response_error( int errcode, int clientfd, uint64_t client_tag );

	int box_client_response_error( int errcode, boost::shared_ptr<HttpRequestPacket> prequest , general_log &log);

	int box_client_response_ok_params(boost::shared_ptr<HttpRequestPacket> prequest, 
		std::map<std::string, std::string> &map_param, general_log &log);

	
	

private:

	int Chk18PaperId (const char *sPaperId);
	//int update_gamelib_default_config(void);
	static void *dispatch_http_request_proc(void *param);
	static void *http_response_proc(void *param);
	static void *timer_stat_proc(void *param);
	void timer_stat_doit(void);
	void dispatch_http_request(void);
	int server_path2id(std::string &str_path);

	static void *sync_sessionid_to_cache_proc(void *param);
	void sync_sessionid_to_cache_doit(void);

	int check_request_queued_timeout( boost::shared_ptr<HttpRequestPacket> prequest);

	void init_server_map(void);

	void message001totrustee(STU_MESSAGE_0001 *pmsg, STU_TRUSTEE_INFO *ptrustee);

	void trustee2json(std::string &str_json, std::vector<STU_TRUSTEE_INFO> &vec_trustee);

	void prevent2json(std::string &str_json, std::vector<STU_PREVENT_INFO> &vec_prevent);

	void attach_loginfo(STU_SESSION_INFO *psession, HttpRequestPacket *prequest,std::string &strsessionid);

	block_queue< boost::shared_ptr<STU_HTTP_RTN> > m_queue_response;

//	threadpoll_manager m_thread_poll_man;
	threadpoll_manager m_thread_poll_logingame_man;
	http_manager m_http_manager;
	databasepool m_databasepool;
	memcache_cli_pool m_memcache_pool;
//	sessionid_man  m_sessionid_man;
//	login_realtime_service m_loginrealtime;
	DECL_LOGGER(logger);
	std::map<std::string, int> m_map_request_path;
	std::map<int, std::string> m_map_err_info;
	std::string m_str_success_static;
	uint64_t m_queue_delay_drop_threashold;

	login_track_service m_login_track_server;

//	ipserver m_ipserver;

//	redis_pool m_redis_pool;


	template_pool<mongo_client> m_mongodb_pool;
	
	int jumpkeylogin_check(std::string str_loginstatus);


	// 启动来源到推广id的映射
	std::map<std::string, std::string> m_map_startsource_promptid;
	void init_map_startsource_promptid(void);
	std::string get_promptid_by_startsource(std::string start_source);
	
	zmq_client_pub m_zmq_client_pub_trustee_notify;
	void zmq_client_pub_trustee_notify_recv(std::string &data);
	std::string m_zmq_router_notify_server_identity;
	std::string m_zmq_service_self_identity;
	void send_agreetrustee_notify(std::string from_gameinternalno, std::string to_gameinternalno,
		std::string message_type, std::string info);

	int master_slave_check(void);

public:

	int get_session_info(std::string &str_sessionid,STU_SESSION_INFO *psessioninfo);

	int set_session_info(std::string &str_sessionid,STU_SESSION_INFO *psessioninfo);

	int session_to_json(std::string &str_json, STU_SESSION_INFO *psessioninfo);

	int json_to_session(std::string &str_json, STU_SESSION_INFO *psessioninfo);

	int get_sessionid_by_gameinternalno(std::string str_gameinternalno, std::string &str_sessionid);

	int set_gameinternalno_sessionid_map(std::string &strsessionid, std::string &str_gameinternalno);
	int set_gameinternalno_sessionid_map(std::string &strsessionid, std::string &str_gameinternalno,
										memcache_client *pmem_client);

	int remove_sessioninfo(std::string &str_sessionid);

	int remove_cache_gameinternalno(std::string gameinternalno);

	int cache_gamelib_get_vec(std::vector<std::string> &vec ,std::string &str_key, redis_client *pclient);

	int cache_gamelib_set_value( std::string &str_key, std::string &str_value, redis_client *pclient);

	int cache_gamelib_del_value(std::string &str_key, std::string &str_value, redis_client *pclient );

	int cache_set_session_tickout(std::string old_session);

	// 处理每一条请求
	void test_process(boost::shared_ptr<HttpRequestPacket> request);

	void Process_loginGame(boost::shared_ptr<HttpRequestPacket> request);

	void Process_rapeLoginGame(boost::shared_ptr<HttpRequestPacket> request);

	void Process_specialLoginGame(boost::shared_ptr<HttpRequestPacket> request);

	void Process_flushGameInfo(boost::shared_ptr<HttpRequestPacket> request);

	void Process_flushGameInfoSpe(boost::shared_ptr<HttpRequestPacket> request);
	
	void Process_slaverLoginGame(boost::shared_ptr<HttpRequestPacket> request);
	
};
