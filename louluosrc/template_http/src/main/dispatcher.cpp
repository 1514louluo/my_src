#include "dispatcher.h"

#include "config_singleton.h"
#include <sys/types.h>
#include "service_stat.h"
IMPL_LOGGER(dispatcher, logger);
dispatcher::dispatcher(void){}
dispatcher::~dispatcher(void){}
 
void dispatcher::init_server_map( void )
{
	//SDUtility::GBKToUTF8()
	std::string param;
 	param = "成功";
 	SDUtility::GBKToUTF8(param);
	m_map_err_info.insert(make_pair(SUCCESS, param));
	param = "参数错误";
	SDUtility::GBKToUTF8(param);
	m_map_err_info.insert(make_pair(PARAM_ERROR, param));

	param = "从redis池中分配对象失败";
	SDUtility::GBKToUTF8(param);
	m_map_err_info.insert(make_pair(GET_REDIS_FROM_POOL_ERROR, param));
	
	param = "系统超时";
	SDUtility::GBKToUTF8(param);
	m_map_err_info.insert(make_pair(INTERNAL_TIMEOUT, param));

	param = "非法请求";
	SDUtility::GBKToUTF8(param);
	m_map_err_info.insert(make_pair(ILLEGAL_ACCESS, param));	

	// 登陆注册协议集
	m_map_request_path.insert(make_pair(GAMEBOX_SERVER_PROPERTY_POST, GAMEBOX_SERVER_PROPERTY_POST_ID));
	m_map_request_path.insert(make_pair(GAMEBOX_SERVER_PROPERTY_DELETE, GAMEBOX_SERVER_PROPERTY_DELETE_ID));
	m_map_request_path.insert(make_pair(GAMEBOX_SERVER_PROPERTY_QUERY, GAMEBOX_SERVER_PROPERTY_QUERY_ID));
	m_map_request_path.insert(make_pair(GAMEBOX_SERVER_PROPERTY_QUERYBATCH, GAMEBOX_SERVER_PROPERTY_QUERYBATCH_ID));
}


void dispatcher::init( void )
{
	int result;
	m_str_success_static = "成功";
	SDUtility::GBKToUTF8(m_str_success_static);
	init_server_map();
	
	// static初始化
	RW_detacher::init_RW_detacher(RW_DETACHER_ALL);

	m_http_manager.set_parent(this);
	m_http_manager.init_http_manager();
	m_thread_poll_man.init_thread_poll();

	SYS_LOG_INFO("service init finished");
	pthread_t threadid;
	result = pthread_create(&threadid, NULL, dispatcher::dispatch_http_request_proc, this);
	if (-1 == result)
	{
		SYS_LOG_ERROR("failed to create thread dispatcher::dispatch_http_request_proc");
		sleep(1);
		exit(0);
		return;
	}
	result = pthread_create(&threadid, NULL, dispatcher::http_response_proc, this);
	if (-1 == result)
	{
		SYS_LOG_ERROR("failed to create thread dispatcher::http_response_proc");
		sleep(1);
		exit(0);
		return;
	}

	const configref& config = config_singleton::get_instance()->get_config();
	std::string zmq_pub_ip = config.getString("zmq_pub_ip", "127.0.0.1");
	int zmq_pub_port = config.getInt("zmq_pub_port", 4444);
	std::string zmq_pub_identity=config.getString("zmq_pub_identity","unkown");

	boost::function<void (std::string &)> zmq_pub_callback = boost::bind(&dispatcher::zmq_pub_func, this, _1);
	result = m_zmq_client_pub.init_zmq_client(zmq_pub_ip, zmq_pub_port, zmq_pub_identity, zmq_pub_callback);
	if(-1 == result)
	{
		SYS_LOG_ERROR("failed to init zmq pub!!!");
	}
}

void dispatcher::zmq_pub_func( std::string &data ){}

void * dispatcher::dispatch_http_request_proc( void *param )
{

	if (param == NULL)
	{
		return (void *)0;
	}
	dispatcher *pthis  = (dispatcher *)param;
	pthis->dispatch_http_request();
	SYS_LOG_INFO("dispatch_http_request_proc thread terminated");

	return (void *)0;
}

void dispatcher::dispatch_http_request( void )
{
	while (true)
	{
		int result;
		boost::shared_ptr<HttpRequestPacket> prequest;
		result = m_request_queue.dequeue_expand(prequest);
		if (result != 1)
		{
			continue;
		}
				
		SYS_LOG_DEBUG("get one request,url="<<prequest->get_request_url());
		int serverid = server_path2id(prequest->m_str_cmdpath);
		if (serverid == -1)
		{
			SYS_LOG_ERROR("serverid not registered or suported, cmd path="<<prequest->m_str_cmdpath);
			continue;
		}
		//prequest->LOGDEBUG();
		prequest->m_request_type = serverid;
		boost::function<void(void)> task = NULL;
		switch (serverid)
		{
			case GAMEBOX_SERVER_PROPERTY_POST_ID:
				task = boost::bind(&dispatcher::Process_post, this, prequest);	
				break;
			case GAMEBOX_SERVER_PROPERTY_DELETE_ID:
				task = boost::bind(&dispatcher::Process_delete, this, prequest);	
				break;
			case GAMEBOX_SERVER_PROPERTY_QUERY_ID:
				task = boost::bind(&dispatcher::Process_query, this, prequest);	
				break;
			case GAMEBOX_SERVER_PROPERTY_QUERYBATCH_ID:
				task = boost::bind(&dispatcher::Process_queryBatch, this, prequest);	
				break; 

			default:
				SYS_LOG_ERROR("process dealing still not supported, cmdpath="<<prequest->m_str_cmdpath<<" serverid="<<serverid);
				http_response_error(ILLEGAL_ACCESS,prequest);
				break;
		}
		if (task != NULL)
		{			
			m_thread_poll_man.commit_task(task);			
		}
	}
}

int dispatcher::server_path2id(std::string &str_path)
{
	std::map<std::string, int>::iterator iter;
	iter = m_map_request_path.find(str_path);
	if (iter == m_map_request_path.end())
	{
		return -1;
	}
	return iter->second;
}

int dispatcher::http_response( HttpResponseHeaderPacket *phttp_response , int clientfd, 
									uint64_t client_unify_tag)
{
	if (phttp_response == NULL)
	{
		return -1;
	}

	boost::shared_ptr< STU_HTTP_RTN > pstu_http_rtn(new STU_HTTP_RTN);
	pstu_http_rtn->clientfd = clientfd;
	pstu_http_rtn->unify_tag = client_unify_tag;
	pstu_http_rtn->str_send = phttp_response->m_str_respond;
	m_queue_response.add_queue(pstu_http_rtn);
	return 0;
}
void * dispatcher::http_response_proc( void *param )
{

	if (param == NULL)
	{
		return (void *)0;
	}

	dispatcher *pthis = (dispatcher *)param;
	while (true)
	{
		int result;
		boost::shared_ptr<STU_HTTP_RTN> pstu_http_rtn;
		result = pthis->m_queue_response.dequeue_expand(pstu_http_rtn);
		if (result != 1)
		{
			continue;
		}
		pthis->m_http_manager.m_httpserver.send_data(pstu_http_rtn->clientfd, pstu_http_rtn->unify_tag,
			pstu_http_rtn->str_send);
		pthis->m_http_manager.m_httpserver.close_client(pstu_http_rtn->clientfd, pstu_http_rtn->unify_tag);

	}
}

int dispatcher::http_response_error( int errcode, boost::shared_ptr<HttpRequestPacket> prequest )
{
	service_stat *pstat = service_stat::singleton();
	std::map<int, std::string>::iterator iter;
	std::map<std::string, std::string> rtn_map;
	iter = m_map_err_info.find(errcode);
	if (iter == m_map_err_info.end())
	{
		SYS_LOG_ERROR("box_client_response_error param not supported, drop response error info ");
		return -1;

	}
	rtn_map.insert(make_pair("result", SDUtility::format(errcode)) );
	rtn_map.insert(make_pair("errinfo", iter->second));
	HttpResponseHeaderPacket respose;
	respose.encode(HTTP_RESPONSE_STATUS_OK, rtn_map);
	std::string str_body;
	map2string(str_body, rtn_map);
	//SYS_LOG_DEBUG("http response: "<<str_body);
	http_response(&respose, prequest->m_clientfd, prequest->m_client_tag);
	
	time_t nowtime = SDUtility::get_cur_millseconds();
	uint64_t processing_periodtime  = nowtime - prequest->m_start_processtime;
//	if (processing_periodtime > 5000)
	if (processing_periodtime > 1000)
	{
		PERFORM_LOG_INFO("process time too long, period millsecs="<<processing_periodtime<<
			" cmd="<<prequest->m_str_GetRequest<<" now="<<nowtime<<"  starttime="<<prequest->m_start_processtime);
	}

	switch (prequest->m_request_type)
	{
	case GAMEBOX_SERVER_PROPERTY_POST_ID:
	case GAMEBOX_SERVER_PROPERTY_DELETE_ID:
	case GAMEBOX_SERVER_PROPERTY_QUERY_ID:
	case GAMEBOX_SERVER_PROPERTY_QUERYBATCH_ID:
		{
			LOGINGAME_LOG(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body
				<<" $$$ ");
		}
		break;
	
	default:
		{
			SERVICE_LOG_INFO(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body
				<<" $$$ ");
		}
		break;
	}
	
	
	return 0;

	//return box_client_response_error(errcode, prequest->m_clientfd, prequest->m_client_tag);

}

int dispatcher::http_response_ok(boost::shared_ptr<HttpRequestPacket> prequest)
{
	service_stat *pstat = service_stat::singleton();
	std::map<std::string, std::string> body;
	body.insert(make_pair("result", SDUtility::format(SUCCESS)));
	body.insert(make_pair("errinfo", m_str_success_static));
	HttpResponseHeaderPacket response;
	std::string str_body;
	map2string(str_body, body);
	response.encode(HTTP_RESPONSE_STATUS_OK, body);
	http_response(&response, prequest->m_clientfd, prequest->m_client_tag);

	uint64_t nowtime = SDUtility::get_cur_millseconds();
	uint64_t processing_periodtime  = nowtime - prequest->m_start_processtime;
//	if (processing_periodtime > 5000)
	if (processing_periodtime > 1000)
	{
		PERFORM_LOG_INFO("process time too long, period millsecs="<<processing_periodtime<<
			" cmd="<<prequest->m_str_cmdpath<<" now="<<nowtime<<"  starttime="<<prequest->m_start_processtime);
	}

	
	switch (prequest->m_request_type)
	{

	case GAMEBOX_SERVER_PROPERTY_POST_ID:
	case GAMEBOX_SERVER_PROPERTY_DELETE_ID:
	case GAMEBOX_SERVER_PROPERTY_QUERY_ID:
	case GAMEBOX_SERVER_PROPERTY_QUERYBATCH_ID:
		{
			LOGINGAME_LOG(""<<prequest->get_request_url()<<"  RESPONSE-->"<<str_body<<
				" $$$  ");
		}
		break;
	
	default:
		{
			SERVICE_LOG_INFO(prequest->m_str_GetRequest<<" $$$$ RESPONSE-->>> "<<str_body
				<<" $$$ ");
		}
		break;
	}
	 
	
	
	STU_REQ_RESP_STAT stat;
	stat.cmd_type = prequest->m_request_type;
	stat.requesttime_millsec = prequest->m_addqueue_time_millsec;
	stat.responsetime_millsec = SDUtility::get_cur_millseconds();
	pstat->add_req_resp_stat(stat);
	return 0;

}


int dispatcher::http_response_ok_params(boost::shared_ptr<HttpRequestPacket> prequest, 
											  std::map<std::string, std::string> &map_param)
{
	service_stat *pstat = service_stat::singleton();
	std::map<std::string, std::string> body;
	body.insert(make_pair("result", SDUtility::format(SUCCESS)));
	body.insert(make_pair("errinfo", "ok"));

	std::map<std::string, std::string>::iterator iter;
	for (iter = map_param.begin(); iter!= map_param.end(); iter++)
	{
		body.insert(make_pair(iter->first, iter->second ));
	}
	HttpResponseHeaderPacket response;
	response.encode(HTTP_RESPONSE_STATUS_OK, body);
	std::string str_body;
	map2string(str_body, body);
	//SYS_LOG_DEBUG("http response="<<str_body);
	http_response(&response, prequest->m_clientfd, prequest->m_client_tag);
	uint64_t nowtime = SDUtility::get_cur_millseconds();
	uint64_t processing_periodtime  = nowtime - prequest->m_start_processtime;
//	if (processing_periodtime > 5000)
	if (processing_periodtime > 1000)
	{
		PERFORM_LOG_INFO("process time too long, period millsecs="<<processing_periodtime<<
			" cmd="<<prequest->m_str_cmdpath<<" now="<<nowtime<<"  starttime="<<prequest->m_start_processtime);
	}
	

	switch (prequest->m_request_type)
	{
	case GAMEBOX_SERVER_PROPERTY_POST_ID:
	case GAMEBOX_SERVER_PROPERTY_DELETE_ID:
	case GAMEBOX_SERVER_PROPERTY_QUERY_ID:
	case GAMEBOX_SERVER_PROPERTY_QUERYBATCH_ID:
			{
				LOGINGAME_LOG(""<<prequest->get_request_url()<<"  RESPONSE-->"<<str_body<<
					" $$$  ");
			}
			break;
		
		default:
			{
				SERVICE_LOG_INFO(prequest->m_str_GetRequest<<" $$$ RESPONSE-->>> "<<str_body
					<<"$$$ ");
			}
			break;
	}

	
	STU_REQ_RESP_STAT stat;
	stat.cmd_type = prequest->m_request_type;
	stat.requesttime_millsec = prequest->m_addqueue_time_millsec;
	stat.responsetime_millsec = SDUtility::get_cur_millseconds();
	
	pstat->add_req_resp_stat(stat);
	return 0;
}

int dispatcher::check_request_queued_timeout( boost::shared_ptr<HttpRequestPacket> prequest )
{	
	// 需要检查队列中请求排队的时间，如果超过一定时间，将丢弃请求，避免导致雪崩效应
	uint64_t currenttime_millsec = SDUtility::get_cur_millseconds();
	prequest->m_dequeue_time_millsec = currenttime_millsec;
	if (currenttime_millsec - prequest->m_addqueue_time_millsec > m_queue_delay_drop_threashold)
	{
		prequest->m_response_time_millsec = currenttime_millsec;
		PERFORM_LOG_INFO("request queueed too long, drop process, response error imediately, cmd="
			<<prequest->m_str_cmdpath<<"queued time="<<currenttime_millsec - prequest->m_addqueue_time_millsec
			<<" threshold="<<m_queue_delay_drop_threashold);
		return -1;
	}
	else
	{
		return 0;
	}

}
void dispatcher::Process_post(boost::shared_ptr<HttpRequestPacket> request)
{
	std::string sql = "insert into serverid_map values(0000243, 01, 02)";
	RW_detacher::do_mysql_opt_write(sql);
	http_response_ok(request);
}

void dispatcher::Process_delete(boost::shared_ptr<HttpRequestPacket> request)
{
	std::string sql = "delete from serverid_map where gameid=0000243";
	RW_detacher::do_mysql_opt_write(sql);
	http_response_ok(request);
}

void dispatcher::Process_query(boost::shared_ptr<HttpRequestPacket> request)
{
	std::string sql = "select * from serverid_map";
	string_vector reply;
	RW_detacher::do_mysql_opt_read(sql, reply);
	BOOST_FOREACH(std::string item, reply)
	{
		printf("%s\n", item.c_str());
	}
	http_response_ok(request);
}

void dispatcher::Process_queryBatch(boost::shared_ptr<HttpRequestPacket> request)
{}












