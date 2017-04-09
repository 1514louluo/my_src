
#include "http_server.h"
#include "monitor_common_format.h"
#include "SDConfigurationSingleton.h"

//#include "http_monitor_core.h"
#include "service_stat.h"
std::string http_server::m_str_startuptime;
std::string http_server::m_str_complile_time;
IMPL_LOGGER(http_server, logger);

http_server::http_server(void)
{
	//m_p_monitor_core = NULL;
	m_str_startuptime = SDUtility::get_current_time();
	m_str_complile_time = __DATE__;
	m_str_complile_time = m_str_complile_time+ "  " + __TIME__;
	
}

http_server::~http_server(void)
{
}



void * http_server::libevent_http_reactor( void *param )
{

	if (param == NULL)
	{
		return (void *)0;
	}
	http_server *pthis = (http_server *)param;
	pthis->libevent_http_reactor_doit();
	MY_LOGERROR("http_server::libevent_http_reactor terminated");
	return (void *)0;


}

void * http_server::libevent_http_reactor_doit( void )
{

	// 需要重写
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	struct event_base *base;
	struct evhttp *http;
	unsigned short port = config.getInt("http_server_port_diagnose", 10000);
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
	
	//struct evhttp *httpd;
	//std::string proxy_listen;
	//proxy_listen = "0.0.0.0";
	//int proxy_port = config.getInt("http_server_port", 10000);
	//int proxy_settings_timeout = config.getInt("http_server_timeout", 30); //http请求超时时间
	////初始化监听ip和端口
	//event_init();
	//httpd = evhttp_start(proxy_listen.c_str(), proxy_port);
	//if (httpd == NULL) 
	//{
	//	MY_LOGERROR("Error: Unable to listen on "<<proxy_listen<<" "<<proxy_port);
	//	exit(1);
	//}
	//evhttp_set_timeout(httpd, proxy_settings_timeout);
	//evhttp_set_gencb(httpd, NULL, NULL);
	//event_dispatch();

	return (void*)0;


}







/*
 *http处理模块
*/
void http_server::api_proxy_handler(struct evhttp_request *req, void *arg) 
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
	 evhttp_add_header(evhttp_request_get_output_headers(req), "name", "helloworld");

	 evb = evbuffer_new();

	 service_stat *pstat=service_stat::singleton();
	 std::string str_echo;
	 std::string key;
	 std::string value;
	
	
	 char charhostname[1024]={0};
	 gethostname(charhostname, 1024);
	 std::string str_hostname;
	 str_hostname = charhostname;
	 //str_echo = str_echo + "machinename:" + str_hostname + end;

	monitor_common_format codec;
	
	key = "machinename";
	value = str_hostname;
	codec.add_kv(key,value);

	key = "curr_conn";
	value = SDUtility::format(pstat->m_tcp_curent_conn);
	codec.add_kv(key,value);

	key="his_conn";
	value = SDUtility::format(pstat->m_tcp_history_conn);
	codec.add_kv(key, value);

	key="cmd_count_60seconds";
	value = SDUtility::format(pstat->m_cmd_counts_60seconds_stat);
	codec.add_kv(key, value);

	key="cpu";
	value = SDUtility::format((uint32_t)pstat->m_cpu_usage);
	codec.add_kv(key,value);

	key = "memory";
	char buffertmp[40]={0};
	sprintf(buffertmp, "%f", pstat->m_mem_usage);
	value = buffertmp;
	codec.add_kv(key,value);

	key = "overload";
	value = SDUtility::format(pstat->m_overload);
	codec.add_kv(key,value);

	codec.get_json_format(str_echo);

	evbuffer_add_printf(evb, str_echo.c_str());










	 evhttp_send_reply(req, HTTP_OK, "OK", evb);
	 evbuffer_free(evb);
	 //释放内存
	 //evhttp_clear_headers(&req);
	 
/*
    //初始化返回客户端的数据缓存
    struct evbuffer *buf;
    buf = evbuffer_new();

   
    char *decode_uri = strdup((char*) evhttp_request_uri(req));

    evkeyval http_query;

	//evkeyvalq queue1;
	//queue1 = new evkeyvalq;
	
	

    //evhttp_parse_query(decode_uri, &http_query);
    free(decode_uri);

    //接收GET表单参数name
    //const char *http_input_name = evhttp_find_header(&http_query, "name"); 

    //处理输出header头
    evhttp_add_header(req->output_headers, "Content-Type", "text/plain");
    evhttp_add_header(req->output_headers, "Connection", "keep-alive");
    evhttp_add_header(req->output_headers, "Cache-Control", "no-cache");

    //处理输出数据
    evbuffer_add_printf(buf, "PROXY VERSION %s\n", VERSION);
    evbuffer_add_printf(buf, "------------------------------\n");
   // evbuffer_add_printf(buf, "YOU PASS NAME: %s\n", http_input_name);

    //返回code 200
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    //释放内存
   //evhttp_clear_headers(&http_query);
   evbuffer_free(buf);


   */
}

void http_server::init( void )
{

	int result;
	pthread_t threadid;
	result=pthread_create(&threadid, NULL, http_server::libevent_http_reactor, this);
	if (result == -1)
	{
		MY_LOGERROR("failed to create  http_server::libevent_http_reactor");
		return;
	}



	
}

//void http_server::set_monitor( http_monitor_core *pparam )
//{
//	if (pparam == NULL)
//	{
//		return;
//	}
//	m_p_monitor_core = pparam;
//
//}


