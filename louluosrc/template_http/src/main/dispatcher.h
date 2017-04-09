#pragma once

#include "block_queue.h"
#include "mycommon.h"
#include "HttpRequestPacket.h"
#include "threadpoll_manager.h"
#include "http_manager.h"
#include <sys/types.h>
#include "commondef.h"
#include "logdef.h"
#include "redis_pool.h"
#include "Clock.h"
#include "RW_detacher.h"
#include "cjson_wrapper.h"
#include "zmq_client_pub.h"

class dispatcher
{
public:
	dispatcher(void);
	~dispatcher(void);

	void init(void);

	int http_response(HttpResponseHeaderPacket *phttp_response, int clientfd, uint64_t client_unify_tag);
	int http_response_ok(boost::shared_ptr<HttpRequestPacket> prequest);
	int http_response_error( int errcode, boost::shared_ptr<HttpRequestPacket> prequest );
	int http_response_ok_params(boost::shared_ptr<HttpRequestPacket> prequest, 
																std::map<std::string, std::string> &map_param);
/*协议*/
	void Process_post(boost::shared_ptr<HttpRequestPacket> request);
	void Process_delete(boost::shared_ptr<HttpRequestPacket> request);
	void Process_query(boost::shared_ptr<HttpRequestPacket> request);
	void Process_queryBatch(boost::shared_ptr<HttpRequestPacket> request);
/*协议*/	


	block_queue< boost::shared_ptr<HttpRequestPacket>  >   m_request_queue;
private:
	static void *dispatch_http_request_proc(void *param);
	void dispatch_http_request(void);
	static void *http_response_proc(void *param);

	void zmq_pub_func( std::string &);
	
	int server_path2id(std::string &str_path);
	int check_request_queued_timeout( boost::shared_ptr<HttpRequestPacket> prequest);
	void init_server_map(void);
	
	block_queue< boost::shared_ptr<STU_HTTP_RTN> >         m_queue_response;
	std::map<std::string, int> m_map_request_path;
	std::map<int, std::string> m_map_err_info;
	
	std::string m_str_success_static;
	uint64_t m_queue_delay_drop_threashold;

	zmq_client_pub  m_zmq_client_pub;
	threadpoll_manager m_thread_poll_man;
	http_manager m_http_manager;
	Clock  *m_clock;

	DECL_LOGGER(logger);	
};




