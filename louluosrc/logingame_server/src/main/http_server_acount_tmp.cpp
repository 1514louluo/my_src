
#include "http_server_acount_tmp.h"

#include "SDConfigurationSingleton.h"

//#include "http_monitor_core.h"
#include "service_stat.h"
std::string http_server_acount_tmp::m_str_startuptime;
std::string http_server_acount_tmp::m_str_complile_time;
IMPL_LOGGER(http_server_acount_tmp, logger);

http_server_acount_tmp::http_server_acount_tmp(void)
{
	//m_p_monitor_core = NULL;
	m_str_startuptime = SDUtility::get_current_time();
	m_str_complile_time = __DATE__;
	m_str_complile_time = m_str_complile_time+ "  " + __TIME__;
	
}

http_server_acount_tmp::~http_server_acount_tmp(void)
{
}



void * http_server_acount_tmp::libevent_http_reactor( void *param )
{

	if (param == NULL)
	{
		return (void *)0;
	}
	http_server_acount_tmp *pthis = (http_server_acount_tmp *)param;
	pthis->libevent_http_reactor_doit();
	MY_LOGERROR("http_server_acount_tmp::libevent_http_reactor terminated");
	return (void *)0;


}

void * http_server_acount_tmp::libevent_http_reactor_doit( void )
{

	// 需要重写
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	struct event_base *base;
	struct evhttp *http;
	unsigned short port = 4500;
	signal(SIGPIPE, SIG_IGN);
	base = event_base_new();
	if (!base) 
	{
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		return (void *)0;
	}
	/* Create a new evhttp object to handle requests. */
	http = evhttp_new(base);
	if (!http)
	{
		fprintf(stderr, "couldn't create evhttp. Exiting.\n");
		return (void *)0;
	}
	evhttp_set_cb(http, "/", api_proxy_handler, NULL);
	evhttp_set_gencb(http, api_proxy_handler, NULL);
	int bind_result;
	bind_result = evhttp_bind_socket(http, "0.0.0.0", port);
	if (bind_result != 0) 
	{
		MY_LOGERROR("couldn't bind to port "<<(int)port<<" Exiting");
		return (void *)0;
	}

	event_base_dispatch(base);
	


	return (void*)0;


}







/*
 *http处理模块
*/
void http_server_acount_tmp::api_proxy_handler(struct evhttp_request *req, void *arg) 
{
	//const char *cmdtype;
	struct evkeyvalq *headers;
	//struct evkeyval *header;
	struct evbuffer *evb;
	const char *request_uri;


	 evhttp_cmd_type cmd_type;
	 // 获取方法类型
	 cmd_type = evhttp_request_get_command(req);
	 switch (cmd_type)
	 {
	 case EVHTTP_REQ_GET:
		 break;
	 default:
		 MY_LOGERROR("unsupported method cmd_type="<<cmd_type);
		 return;
	 }

	 // 获取uri
	 request_uri = evhttp_request_get_uri(req);
	 // 获取header
	 headers = evhttp_request_get_input_headers(req);
	// for (header = headers->tqh_first; header;  header = header->next.tqe_next)
	// {
			 //printf("  %s: %s\n", header->key, header->value);
	// }


	 evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/plain");
	 evhttp_add_header(evhttp_request_get_output_headers(req), "Connection", "keep-alive");
	 evhttp_add_header(evhttp_request_get_output_headers(req), "Cache-Control", "no-cache");
	 //evhttp_add_header(evhttp_request_get_output_headers(req), "name", "helloworld");

	 evb = evbuffer_new();

	 std::string str_echo;
	 str_echo = "500";
	 
	
	 evbuffer_add_printf(evb, str_echo.c_str());










	 evhttp_send_reply(req, HTTP_OK, "OK", evb);
	 evbuffer_free(evb);
	 
}

void http_server_acount_tmp::init( void )
{

	int result;
	pthread_t threadid;
	result=pthread_create(&threadid, NULL, http_server_acount_tmp::libevent_http_reactor, this);
	if (result == -1)
	{
		MY_LOGERROR("failed to create  http_server_acount_tmp::libevent_http_reactor");
		return;
	}



	
}

