
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
	 std::string end;
	 end = " \n";
	 //std::map<std::string, std::string> my_kv_map;
	 char charhostname[1024]={0};
	 gethostname(charhostname, 1024);
	 std::string str_hostname;
	 str_hostname = charhostname;
	 str_echo = str_echo + "machinename:" + str_hostname + end;
/*
	 //----------------------machine info --------------------------------
	 key = "server name: ";
	 value = "gamebox_gate";
	 str_echo = str_echo + key + value + end;

	 key = "local ip: ";
	 value = pstat->m_str_local_ip;
	 str_echo = str_echo + key + value + end;

	 key = "server start time: ";
	 value = m_str_startuptime;
	 str_echo = str_echo + key + value  + end;

	 key = "complie time: ";
	 value = m_str_complile_time;
	 str_echo = str_echo + key + value  + end;

	 // --------------------machine status---------------------------
	 str_echo = str_echo + end;
	 key = "cpu percentage: ";
	 value = SDUtility::format(pstat->m_cpu);
	 str_echo = str_echo + key + value + end;

	 key = "mem usage: ";
	 char charfloat[30]={0};
	 sprintf(charfloat, "%f", pstat->m_mem);
	 value = charfloat;
	 value = value + " %";
	 str_echo = str_echo + key + value + end;

	 key = "overload: ";
	 value = SDUtility::format(pstat->m_overload);
	 str_echo = str_echo + key + value + end;
	 str_echo = str_echo + end + end;


	 //------------------get total client connection--------------------------------
	 uint64_t total_client = pstat->m_current_total_client;
	 uint64_t current_client = pstat->m_current_conn_count;
	 uint64_t s60_new_conn = pstat->m_60s_client_conn;

	 
	 
	 key = "total connection since start up: ";
	 value = SDUtility::format(total_client);
	 str_echo = str_echo + key + value + end;

	 key = "current avaliable connection: ";
	 value = SDUtility::format(current_client);
	 str_echo = str_echo + key + value + end;

	 key = "60 seconds with new connection: ";
	 value = SDUtility::format(s60_new_conn);
	 str_echo = str_echo + key + value + end;

	 key = "1 second with new connection: ";
	 value = SDUtility::format(   (int)(((double)s60_new_conn)/60.0)  );
	 str_echo = str_echo + key + value + end;

	 // ------------------------get db stat------------------------------------
	 str_echo = str_echo + end + end;
	 key = "60 second with db read count: ";
	 value = SDUtility::format( pstat->m_db_read_60s_count );
	  str_echo = str_echo + key + value + end;

	 key = "60 second with db write count: ";
	 value = SDUtility::format( pstat->m_db_write_60s_count );
	 str_echo = str_echo + key + value + end;

	 key = "60 second with db read delay millsec: ";
	 value = SDUtility::format( pstat->m_db_read_60s_delay );
	 str_echo = str_echo + key + value + end;

	 key = "60 second with db write delay millsec: ";
	 value = SDUtility::format( pstat->m_db_write_60s_delay );
	 str_echo = str_echo + key + value + end;

	 //-------------  get mongodb  stat -----------------
	 str_echo = str_echo + end + end;
	 key = "60 second with monogodb read count: ";
	 value = SDUtility::format( pstat->m_mongo_read_60s_count );
	 str_echo = str_echo + key + value + end;

	 key = "60 second with monogodb write count: ";
	 value = SDUtility::format( pstat->m_mongo_write_60s_count );
	 str_echo = str_echo + key + value + end;

	 key = "60 second with monogodb read delay millsec: ";
	 value = SDUtility::format( pstat->m_mongo_read_60s_delay );
	 str_echo = str_echo + key + value + end;

	 key = "60 second with monogodb write delay millsec: ";
	 value = SDUtility::format( pstat->m_mongo_write_60s_delay );
	 str_echo = str_echo + key + value + end;

	 // ------------------memcache stat-----------------
	  str_echo = str_echo + end + end;
	 key = "60 second memcache read count: ";
	 value = SDUtility::format(pstat->m_memcache_read_count_60s);
	 str_echo = str_echo + key + value + end;
	 key = "60 second memcache read delay: ";
	 value = SDUtility::format(pstat->m_memcache_read_delay_60s);
	 str_echo = str_echo + key + value + end;
	 key = "60 second memcache write count: ";
	 value = SDUtility::format(pstat->m_memcache_write_count_60s);
	 str_echo = str_echo + key + value + end;
	 key = "60 second memcache write delay: ";
	 value = SDUtility::format(pstat->m_memcache_write_delay_60s);
	 str_echo = str_echo + key + value + end;


	 // ------------------------get request response stat -----------------------
	  str_echo = str_echo  + end;
	  key = "60s request average delay: ";
	  value  =SDUtility::format(pstat->m_req_resp_delay_millsec);
	  str_echo = str_echo + key + value + end;
	 */
	
	//----------------------machine info --------------------------------
	monitor_common_format codec;
	key = "server_name";
	value = "gamebox_gate";
	codec.add_kv(key,value);

	key = "local_ip";
	value = pstat->m_str_local_ip;
	codec.add_kv(key,value);

	key = "server_start_time";
	value = m_str_startuptime;
	codec.add_kv(key,value);

	key = "complie_time";
	value = m_str_complile_time;
	codec.add_kv(key,value);

// --------------------machine status---------------------------
	
	key = "cpu_percentage";
	value = SDUtility::format(pstat->m_cpu);
	codec.add_kv(key,value);

	key = "mem_usage";
	char charfloat[30]={0};
	sprintf(charfloat, "%f", pstat->m_mem);
	value = charfloat;
	value = value + " %";
	codec.add_kv(key,value);

	key = "overload";
	value = SDUtility::format(pstat->m_overload);
	codec.add_kv(key,value);


//------------------get total client connection--------------------------------
	uint64_t total_client = pstat->m_current_total_client;
	uint64_t current_client = pstat->m_current_conn_count;
	uint64_t s60_new_conn = pstat->m_60s_client_conn;



	key = "total_connection_since_start_up";
	value = SDUtility::format(total_client);
	codec.add_kv(key,value);

	key = "current_avaliable_connection";
	value = SDUtility::format(current_client);
	codec.add_kv(key,value);

	key = "60_seconds_with_new_connection";
	value = SDUtility::format(s60_new_conn);
	codec.add_kv(key,value);

	key = "1_second_with_new_connection";
	value = SDUtility::format(   (int)(((double)s60_new_conn)/60.0)  );
	codec.add_kv(key,value);

// ------------------------get db stat------------------------------------
	
	key = "60_second_with_db_read_count";
	value = SDUtility::format( pstat->m_db_read_60s_count );
	codec.add_kv(key,value);

	key = "60_second_with_db_write_count";
	value = SDUtility::format( pstat->m_db_write_60s_count );
	codec.add_kv(key,value);

	key = "60_second_with_db_read_delay_millsec";
	value = SDUtility::format( pstat->m_db_read_60s_delay );
	codec.add_kv(key,value);

	key = "60_second_with_db_write_delay_millsec";
	value = SDUtility::format( pstat->m_db_write_60s_delay );
	codec.add_kv(key,value);

	//-------------  get mongodb  stat -----------------
	//str_echo = str_echo + end + end;
	key = "60_second_with_monogodb_read_count";
	value = SDUtility::format( pstat->m_mongo_read_60s_count );
	codec.add_kv(key,value);

	key = "60_second_with_monogodb_write_count";
	value = SDUtility::format( pstat->m_mongo_write_60s_count );
	codec.add_kv(key,value);

	key = "60_second_with_monogodb_read_delay_millsec";
	value = SDUtility::format( pstat->m_mongo_read_60s_delay );
	codec.add_kv(key,value);

	key = "60_second_with_monogodb_write_delay_millsec";
	value = SDUtility::format( pstat->m_mongo_write_60s_delay );
	codec.add_kv(key,value);

	// ------------------memcache stat-----------------
	//str_echo = str_echo + end + end;
	key = "60_second_memcache_read_count";
	value = SDUtility::format(pstat->m_memcache_read_count_60s);
	codec.add_kv(key,value);
	key = "60_second_memcache_read_delay";
	value = SDUtility::format(pstat->m_memcache_read_delay_60s);
	codec.add_kv(key,value);
	key = "60_second_memcache_write_count";
	value = SDUtility::format(pstat->m_memcache_write_count_60s);
	codec.add_kv(key,value);
	key = "60_second_memcache_write_delay";
	value = SDUtility::format(pstat->m_memcache_write_delay_60s);
	codec.add_kv(key,value);


	// ------------------------get request response stat -----------------------
	//str_echo = str_echo  + end;
	key = "60s_request_average_delay";
	value  =SDUtility::format(pstat->m_req_resp_delay_millsec);
	codec.add_kv(key,value);

	//------------------ loginresult stat--------------------
	key = "loginresult_fail_rate";
	value = SDUtility::format(pstat->m_loginresult_fail_rate);
	codec.add_kv(key,value);

	//--------------------logingame result stat ------------------
	key = "logingame_result_fail_rate";
	value = SDUtility::format(pstat->m_loginbame_result_fail_rate);
	codec.add_kv(key,value);

	// ------------------logingame duration stat -------------------
	key = "logingame_duration_rate";
	value = SDUtility::format(pstat->m_logingame_duration_stat);
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


