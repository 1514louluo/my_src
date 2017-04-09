#pragma once

#include "HttpRequestPacket.h"
#include "threadpoll_manager.h"
#include "http_manager.h"
#include "commondef.h"
#include "global_var.h"

#include "business_event.h"
#include "cond_event.h"
#include "response_event.h"
#include "task_event.h"

class dispatcher
{
public:
	dispatcher(void);
	~dispatcher(void);

	void init(void);

	block_queue< boost::shared_ptr<HttpRequestPacket>  > m_request_queue;

	int http_response(HttpResponseHeaderPacket *phttp_response, int clientfd, 
							uint64_t client_unify_tag);

	int http_response_ok(boost::shared_ptr<HttpRequestPacket> prequest);

	int http_response_error( int errcode, boost::shared_ptr<HttpRequestPacket> prequest );

	int http_response_ok_params(boost::shared_ptr<HttpRequestPacket> prequest, 
		std::map<std::string, std::string> &map_param);
	
/*协议*/
	void Process_touchBoxAction(boost::shared_ptr<HttpRequestPacket> request);
	void Process_flushFilterConfig(boost::shared_ptr<HttpRequestPacket> request);
/*协议*/	

private:
	static void *dispatch_http_request_proc(void *param);
	static void *http_response_proc(void *param);

	void dispatch_http_request(void);
	int server_path2id(std::string &str_path);
	int check_request_queued_timeout( boost::shared_ptr<HttpRequestPacket> prequest);
	void init_server_map(void);

	static void *simple_timer_proc(void *param);
	void timer_stat_doit( void );

	int touch_core(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param);

	block_queue< boost::shared_ptr<STU_HTTP_RTN> > m_queue_response;
	
	threadpoll_manager m_thread_poll_touch_man;
	http_manager m_http_manager;
	std::map<std::string, int> m_map_request_path;
	std::map<int, std::string> m_map_err_info;
	std::string m_str_success_static;
	uint64_t m_queue_delay_drop_threashold;
	
	std::vector<base_event *> m_events;
	

	DECL_LOGGER(logger);
	
};
