#include "dispatcher.h"
#include "SDConfigurationSingleton.h"
IMPL_LOGGER(dispatcher, logger);


dispatcher::dispatcher(void)
{
}

dispatcher::~dispatcher(void)
{
}

void dispatcher::init( void )
{
	srand(time(NULL));
	int result;
	redis_client::INIT_REDIS_CLIENT();
	memcache_client::INIT_MEMCACHED_CLIENT_MODULE();
	tokenid_service::INIT_TOKENID_SERVICE();
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	m_zmq_router_app_server_identity = config.getString("zmq_router_app_server_identity","unkown");
	m_tcpserver_man.setparent(this);
	m_tcpserver_man.init_tcpserver_manager();
	m_thread_pool.init_thread_poll();
	m_thread_pool_http.init_thread_poll();

	boost::function<void (std::string &)> func_pub=boost::bind(&dispatcher::zmq_pub_recv_callback, this, _1);
	boost::function<void (std::string &)> func_sub=boost::bind(&dispatcher::zmq_sub_recv_callback, this, _1);
	
	std::string zmq_service_pub_ip=config.getString("zmq_service_pub_ip","0.0.0.0");
	int zmq_service_pub_port=config.getInt("zmq_service_pub_port",0);
	std::string zmq_service_sub_ip=config.getString("zmq_service_sub_ip","0.0.0.0");
	int zmq_service_sub_port=config.getInt("zmq_service_sub_port",0);
	m_zmq_service_self_identity=config.getString("zmq_service_self_identity","unknown");
	//m_zmq_router_notify_server_identity = config.getString("zmq_router_notify_server_identity","unkown");
	//m_zmq_router_app_server_identity = config.getString("zmq_router_app_server_identity","unkown");
	result = m_zmq_client_pub.init_zmq_client(zmq_service_pub_ip,zmq_service_pub_port, "notify_adapter",func_pub);
	if (result == -1)
	{
		MY_LOGERROR("failed to start zmq pub");
		return;
	}
	result = m_zmq_client_sub.init_zmq_client(zmq_service_sub_ip,zmq_service_sub_port, 
											m_zmq_service_self_identity,func_sub);
	if (result == -1)
	{
		MY_LOGERROR("failed to start zmq sub");
		return;
	}
	

	int redis_pool_count = config.getInt("redis_pool_client_count",10);
	int i=0;
	for (i=0; i<redis_pool_count; i++)
	{
		redis_client *pclient = new redis_client;
		m_redis_pool.install_obj(pclient);
	}

	int memcache_pool_count = config.getInt("memcache_pool_client_count", 10);
	for (i=0; i<memcache_pool_count; i++)
	{
		memcache_client *pclient = new memcache_client;
		m_memcache_pool.install_obj(pclient);
	}

	
	pthread_t threadid;
	result = pthread_create(&threadid, NULL, dispatcher::adapter_cmd_proc, this);
	if (result == -1)
	{
		MY_LOGERROR("failed to create thread adapter_cmd_proc");
		return;
	}

	result  =pthread_create(&threadid, NULL, dispatcher::http_request_proc, this);
	if (result == -1)
	{
		MY_LOGERROR("failed to create thread http_request_proc");
		return;
	}
	m_http_manager.set_parent(this);
	result = m_http_manager.init_http_manager();
	if (result == -1)
	{
		MY_LOGERROR("init_http_manager failed");
		return;
	}
	MY_LOGDEBUG("dispatcher init finished");

}

void * dispatcher::adapter_cmd_proc( void *param )
{
	if (param == NULL)
	{
		return (void *)NULL;
	}
	dispatcher *pthis = (dispatcher *)param;
	pthis->adapter_cmd_doit();
	MY_LOGERROR("adapter_cmd_proc terminated");
	return (void *)NULL;		
}

void dispatcher::adapter_cmd_doit( void )
{
	service_stat *pstat = service_stat::singleton();
	while (true)
	{
		int result;
		boost::shared_ptr<STU_MSG_INFO> pstu_msg_info;
		result = m_queue_adapter_cmd.dequeue_expand(pstu_msg_info);
		if (result != 1)
		{
			continue;
		}
		
		pstat->add_cmd_count();
		boost::function<void (void) > task=NULL;
		if (pstu_msg_info->content_path == CONTENT_PATH_AUTH)
		{
			task = boost::bind(&dispatcher::process_auth, this,pstu_msg_info);
		}
		else if (pstu_msg_info->content_path == CONTENT_PATH_GETKEY)
		{
			task = boost::bind(&dispatcher::process_getkey, this,pstu_msg_info);
		}
		else if (pstu_msg_info->content_path == CONTENT_PATH_OFFLINE)
		{
			task = boost::bind(&dispatcher::process_offline, this,pstu_msg_info);
		}
		else if (pstu_msg_info->content_path == CONTENT_PATH_HEARTBEAT)
		{
			task = boost::bind(&dispatcher::process_heartbeat, this,pstu_msg_info);
		}
		//else if (pstu_msg_info->content_path == CONTENT_PATH_CLI_QUERY_TRUSTEE)
		//{
		//	task = boost::bind(&dispatcher::process_cli_query_trustee, this,pstu_msg_info);
		//}
		else
		{
			MY_LOGDEBUG("uninterested cmd for adapter, transport to appserver,  path, content="<<pstu_msg_info->content);
			std::string tempcontent;
			tempcontent = pstu_msg_info->content;
			tempcontent = tempcontent+"&tokenid="+pstu_msg_info->tokenid;

			m_zmq_client_pub.send_data(tempcontent, m_zmq_router_app_server_identity);
			continue;
			
		}

		if (task != NULL)
		{
			m_thread_pool.commit_task(task);
		}

		
		
		


	}

}

void dispatcher::process_auth( boost::shared_ptr<STU_MSG_INFO> prequest )
{
	MY_LOGDEBUG("dispatcher::process_auth content="<<prequest->content);
	protocol_codec pro_codec;
	message_codec msgcodec;
	std::string rtn_buffer;
	std::string rtn_content;
	msgcodec.decode_content(prequest->content);
	std::string seqid = get_vaule("seqid",msgcodec.m_content_map);
	std::string gameinternalno = get_vaule("gameinternalno", msgcodec.m_content_map);
	std::string uid = get_vaule("uid",msgcodec.m_content_map);
	//std::string sessionid = get_vaule("sessionid", msgcodec.m_content_map);
	std::string randomkey = get_vaule("key", msgcodec.m_content_map);
	
	if (randomkey.empty())
	{
		MY_LOGERROR("gameinternalno.empty() || sessionid.empty() || randomkey.empty() "<<prequest->content);
		msgcodec.encode_auth(false,rtn_content,seqid,"");
		pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
		m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
		m_tcpserver_man.reply_close(prequest->fd, prequest->tag);
		return;
	}

	int result;

	result = query_randomkey_cache(randomkey);
	if (result != 1)
	{
		MY_LOGERROR("invalid randomkey");
		msgcodec.encode_auth(false,rtn_content,seqid,"");
		pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
		m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
		m_tcpserver_man.reply_close(prequest->fd, prequest->tag);
		return;
	}
	remove_randomkey_cache(randomkey);

	//STU_SESSION_INFO stu_session_info;
	//result = get_session_info(sessionid, &stu_session_info);
	//if (result == -1)
	//{
	//	MY_LOGERROR("failed to get session from memcache, cache error");
	//	return;
	//}
	//if (result == 0)
	//{
	//	MY_LOGERROR("exist no sessionid from memcache, sessionid="<<sessionid);
	//	msgcodec.encode_auth(false,rtn_content);
	//	pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
	//	m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
	//	m_tcpserver_man.reply_close(prequest->fd, prequest->tag);
	//	//m_tcpserver_man.m_tcpserver.send_data(prequest->fd, prequest->tag, rtn_content);
	//	//m_tcpserver_man.m_tcpserver.close_socket(prequest->fd, prequest->tag);
	//	return;
	//}
 
	// session 存在， 需要验证gameinternalno
	//if (stu_session_info.gameinternalno != gameinternalno)
	//{
	//	MY_LOGERROR("session exist, but gameinternalno not equal, stu_session_info.gameinternalno="<<
	//		stu_session_info.gameinternalno<<"  gameinternalno="<<gameinternalno);
	//	msgcodec.encode_auth(false,rtn_content);
	//	pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
	//	m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
	//	m_tcpserver_man.reply_close(prequest->fd, prequest->tag);
	//	//m_tcpserver_man.m_tcpserver.send_data(prequest->fd, prequest->tag, rtn_content);
	//	//m_tcpserver_man.m_tcpserver.close_socket(prequest->fd, prequest->tag);
	//	return;
	//}
	
	// 需要申请tokenid
	std::string tokenid;
	tokenid_service tokenid_server;
	result = tokenid_server.fetch_tokenid(uid,gameinternalno, tokenid);
	if (result == -1  || tokenid.empty())
	{
		MY_LOGERROR("failed to fetch tokenid from tokenid server");
		msgcodec.encode_auth(false,rtn_content,seqid,"");
		pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
		m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
		m_tcpserver_man.reply_close(prequest->fd, prequest->tag);
		return;

	}
	


	msgcodec.encode_auth(true,rtn_content, seqid,tokenid);
	pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
	m_tcpserver_man.reply_auth_ok(prequest->fd, prequest->tag, tokenid);
	m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
	MY_LOGDEBUG("socket auth finshed,gameinternalno="<<gameinternalno<<"  "<<"  uid="<<uid<<"  tokenid="<<tokenid);
	//m_tcpserver_man.m_tcpserver.send_data(prequest->fd, prequest->tag, rtn_buffer);



}


int dispatcher::json_to_session( std::string &str_json, STU_SESSION_INFO *psessioninfo )
{
	cJSON *jsonobj = cJSON_Parse(str_json.c_str());
	if (jsonobj == NULL)
	{
		MY_LOGERROR("failed to dispatcher::json_to_session");
		return -1;
	}

	cJSON *item=NULL;

	item = cJSON_GetObjectItem(jsonobj, "username");
	if (item != NULL){  psessioninfo->username = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "password");
	if (item != NULL){	psessioninfo->password = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "clientip");
	if (item != NULL){	psessioninfo->clientip = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "gameinternalno");
	if (item != NULL){	psessioninfo->gameinternalno = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "idcardno");
	if (item != NULL){	psessioninfo->idcardno = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "gameid");
	if (item != NULL){	psessioninfo->gameid = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "useloginstate");
	if (item != NULL){	psessioninfo->useloginstate = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "heartbeaturl");
	if (item != NULL){	psessioninfo->heartbeaturl = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "comefrom");
	if (item != NULL){	psessioninfo->comefrom = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "xlinternalno");
	if (item != NULL){	psessioninfo->xlinternalno = item->valuestring;}

	item = cJSON_GetObjectItem(jsonobj, "startsource");
	if (item != NULL){	psessioninfo->startsource = item->valuestring;}

	cJSON_Delete(jsonobj);

	return 0;

}



int dispatcher::get_session_info( std::string &str_sessionid,STU_SESSION_INFO *psessioninfo )
{
	std::string session_with_prefix;
	session_with_prefix = PREFIX_SESSIONID;
	session_with_prefix = session_with_prefix + str_sessionid;
	int fetch_result;
	template_guard<memcache_client> guard(&m_memcache_pool);
	fetch_result = guard.fetch_obj_from_pool();
	if (fetch_result == -1)
	{
		return -1;
	}
	memcache_client *pmemclient = guard.get();
	std::string str_value;
	//uint64_t starttime = SDUtility::get_cur_millseconds();
	fetch_result  = pmemclient->get_value(session_with_prefix,str_value);
	//uint64_t endtime = SDUtility::get_cur_millseconds();
	
	if (fetch_result == -1)
	{
		return -1;
	}

	if (fetch_result == 0)
	{
		return 0;
	}

	//-----------------------针对新旧类型sessionid进行处理---------------------
	std::vector<std::string> vec;
	boost::split(vec, str_value, boost::is_any_of(";"));
	if (vec.size() < 8)
	{ //json 格式
		//MY_LOGERROR("memcache value error, value="<<str_value);
		//return -1;
		int result = json_to_session(str_value, psessioninfo);
		if (result == -1)
		{
			return -1;
		}
		return 1;
	}
	else
	{
		psessioninfo->username=vec[0];
		psessioninfo->password=vec[1];
		psessioninfo->clientip=vec[2];
		psessioninfo->gameinternalno=vec[3];
		psessioninfo->idcardno=vec[4];
		psessioninfo->gameid=vec[5]; 
		psessioninfo->useloginstate=vec[6];
		psessioninfo->heartbeaturl=vec[7];
		if (psessioninfo->idcardno == "*")
		{
			psessioninfo->idcardno.clear();
		}
	}



	return 1;

}

void dispatcher::process_cli_query_trustee( boost::shared_ptr<STU_MSG_INFO> prequest )
{
	MY_LOGDEBUG("process process_cli_query_trustee, content="<<prequest->content);
	// 该消息需要丢给notify_server进行处理
	router_msg_service server;
	std::string str_msg;
	server.adapter_to_notify_server_encode_json("cli",m_zmq_service_self_identity,
		prequest->tokenid, prequest->content, str_msg);
	m_zmq_client_pub.send_data(str_msg, m_zmq_router_app_server_identity);
	MY_LOGDEBUG("send msg to notify_server, msg="<<str_msg);


}

void dispatcher::zmq_sub_recv_callback( std::string &str_data )
{
	MY_LOGDEBUG("zmq sub recv data,  "<<str_data);
	//cjson_wrapper wrapper(str_data);

	std::string tokenid;
	std::string reply_content;
	std::map<std::string, std::string> map_recv;
	string2map(str_data, map_recv);
	//wrapper.get_string("tokenid", tokenid);
	//wrapper.get_string("content",reply_content);
	tokenid = get_vaule("tokenid", map_recv);
	if (tokenid.empty())
	{
		MY_LOGERROR("tokenid empty, drop send msg to client");
		return;
	}
	// 检查tokenid是不是对应自己这个adapter所管理的
	
	if (tokenid.find(tokenid_service::m_server_domain)==std::string::npos)
	{
		MY_LOGERROR("tokenid not maintained in this adapter, tokenid_service::m_server_domain="<<tokenid_service::m_server_domain<<
			"  tokenid="<<tokenid);
		return;
	}
	protocol_codec codec;
	std::string sendbuffer;
	codec.encode_buffer(0, str_data,sendbuffer);
	m_tcpserver_man.reply_data_identity(tokenid, sendbuffer);
	

}

void dispatcher::zmq_pub_recv_callback( std::string &str_data )
{

}

void dispatcher::process_getkey( boost::shared_ptr<STU_MSG_INFO> prequest )
{
	int result;
	MY_LOGDEBUG("process_getkey, request="<<prequest->content);
	protocol_codec pro_codec;
	message_codec msgcodec;
	std::string rtn_buffer;
	std::string rtn_content;
	std::string randomkey;
	msgcodec.decode_content(prequest->content);
	std::string gameinternalno = get_vaule("gameinternalno", msgcodec.m_content_map);
	//std::string sessionid = get_vaule("sessionid", msgcodec.m_content_map);

	/*if (gameinternalno.empty() || sessionid.empty())
	{
		MY_LOGERROR("process_getkey  invalid param");
		msgcodec.encode_getkey_rtn(false, randomkey, rtn_content);
		pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
		m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
		m_tcpserver_man.reply_close(prequest->fd, prequest->tag);
		return;
	}

	STU_SESSION_INFO stu_session_info;
	int result;
	result = get_session_info(sessionid, &stu_session_info);
	if (result == -1  || result == 0)
	{
		MY_LOGERROR("process_getkey  invalid sessionid");
		msgcodec.encode_getkey_rtn(false, randomkey, rtn_content);
		pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
		m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
		m_tcpserver_man.reply_close(prequest->fd, prequest->tag);
		return;
	}

	if (stu_session_info.gameinternalno != gameinternalno)
	{
		MY_LOGERROR("process_getkey UN-EQUAL gameinternalno");
		msgcodec.encode_getkey_rtn(false, randomkey, rtn_content);
		pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
		m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
		m_tcpserver_man.reply_close(prequest->fd, prequest->tag);
		return;
	}
	*/

	std::string hashbefore;
	hashbefore = gameinternalno+SDUtility::format( (uint64_t)random() )+SDUtility::format((uint64_t)time(NULL)) ;
	randomkey = SDUtility::get_md5(hashbefore);
	//randomkey = SDUtility::hash(  hashbefore   );
	
	// 将randomkey添加到redis中，并设置过期时间
	result = set_randomkey_cache(randomkey);
	if (result == -1)
	{
		MY_LOGERROR("process_getkey cache error");
		msgcodec.encode_getkey_rtn(false, randomkey, rtn_content);
		pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
		m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);
		m_tcpserver_man.reply_close(prequest->fd, prequest->tag);
		return;
	}

	MY_LOGDEBUG("process_getkey finished");
	msgcodec.encode_getkey_rtn(true, randomkey, rtn_content);
	pro_codec.encode_buffer(0, rtn_content, rtn_buffer);
	m_tcpserver_man.reply_data(prequest->fd, prequest->tag, rtn_buffer);

	return;


}

int dispatcher::set_randomkey_cache( std::string key )
{
	template_guard<redis_client>  redis_guard(&m_redis_pool);
	int result;
	result = redis_guard.fetch_obj_from_pool();
	if (result == -1)
	{
		return -1;
	}
	redis_client *pclient = redis_guard.get();
	if (pclient == NULL)
	{
		return -1;
	}
	std::string value;
	value = "0";
	std::string prefix_key;
	prefix_key = PREFIX_RANDOMKEY;
	prefix_key = prefix_key+key;
	MY_LOGDEBUG("redis setkey: key="<<prefix_key<<"  value="<<value);
	result = pclient->set_value(prefix_key, value);
	if (result == -1)
	{
		return -1;
	}
	pclient->set_expire(prefix_key, 120);
	return 0;

}

int dispatcher::remove_randomkey_cache( std::string key )
{
	template_guard<redis_client> guard(&m_redis_pool);
	int result;
	result = guard.fetch_obj_from_pool();
	if (result == -1)
	{
		return -1;
	}
	redis_client *pclient = guard.get();
	if (pclient == NULL)
	{
		return -1;
	}
	std::string prefix_key;
	prefix_key = PREFIX_RANDOMKEY;
	prefix_key = prefix_key+key;
	pclient->del_value(prefix_key);
	return 0;

}

int dispatcher::query_randomkey_cache( std::string key )
{
	template_guard<redis_client> guard(&m_redis_pool);
	int result;
	result = guard.fetch_obj_from_pool();
	if (result == -1)
	{
		return -1;
	}
	redis_client *pclient = guard.get();
	if (pclient == NULL)
	{
		return -1;
	}
	std::string prefix_key;
	prefix_key = PREFIX_RANDOMKEY;
	prefix_key = prefix_key+key;
	std::string value;

	result = pclient->get_value(prefix_key,value);
	if (result != 0 && result !=1  && result != -1)
	{
		return -1;
	}
	return result;
	

}

void dispatcher::process_offline( boost::shared_ptr<STU_MSG_INFO> prequest )
{
	std::map<std::string, std::string> mapparams;
	mapparams.insert(std::make_pair("path","/svr/notify_adapter/presence/post"));
	mapparams.insert(std::make_pair("type","offline"));
	mapparams.insert(std::make_pair("tokenid",prequest->tokenid));
	std::string data;
	map2string(data, mapparams);
	
	m_zmq_client_pub.send_data(data, m_zmq_router_app_server_identity);

	//tokenid_service server;
	//server.remove_tokenid(prequest->tokenid);
	MY_LOGDEBUG("process_offline finished, data="<<data);

}

void dispatcher::process_heartbeat( boost::shared_ptr<STU_MSG_INFO> prequest )
{
	std::map<std::string, std::string> mapparams;
	mapparams.insert(std::make_pair("path","/notify/token_manager/heartbeat/post"));
	mapparams.insert(std::make_pair("tokenid",prequest->tokenid));

	std::string data;
	map2string(data, mapparams);
	m_zmq_client_pub.send_data(data, m_zmq_router_app_server_identity);
	MY_LOGDEBUG("process_heartbeat finished, data="<<data);
	

}

void * dispatcher::http_request_proc( void *param )
{
	if (param == NULL)
	{
		return (void *)param;

	}
	dispatcher *pthis = (dispatcher *)param;
	pthis->http_request_doit();
	MY_LOGERROR("http_request_proc terminated");
	return (void *)NULL;

}

void dispatcher::http_request_doit( void )
{

	while(true)
	{
		int result;
		boost::shared_ptr<HttpRequestPacket> request;
		result = m_http_request_queue.dequeue_expand(request);
		if (result != 1)
		{
			continue;
		}
		
		if (request->m_str_cmdpath == PATH_BROADCAST)
		{
			boost::function<void (void)> func=boost::bind(&dispatcher::http_process_boadcast, this, request);
			m_thread_pool_http.commit_task(func);
			continue;
		}
		else
		{
			HttpResponseHeaderPacket package;
			std::map<std::string, std::string> mapreturn;
			package.encode(404,mapreturn);
			m_http_manager.m_httpserver.send_data(request->m_clientfd, request->m_client_tag, package.m_str_respond);
			m_http_manager.m_httpserver.close_client(request->m_clientfd, request->m_client_tag);
			continue;
		}


	}

}

void dispatcher::http_process_boadcast( boost::shared_ptr<HttpRequestPacket> request )
{
	std::string broadcastInfo=get_vaule("broadcastInfo", request->mapParam);
	if (broadcastInfo.empty())
	{	
		http_reply_ok(request);
		return;
		
	}

	// 将消息广播给所有终端
	SDUtility::URLDecode(broadcastInfo);
	std::string strboadcast;
	strboadcast="path=/svr/notify/adapter/boadcast&msgcontent="+broadcastInfo;
	protocol_codec codec;
	std::string buffer;
	codec.encode_buffer(0, strboadcast, buffer);
	m_tcpserver_man.boadcast(buffer);
	http_reply_ok(request);

}

void dispatcher::http_reply_ok( boost::shared_ptr<HttpRequestPacket> request )
{
	HttpResponseHeaderPacket package;
	std::map<std::string, std::string> mapreturn;
	mapreturn.insert(std::make_pair("result","1"));
	mapreturn.insert(std::make_pair("errinfo","ok"));
	package.encode(200,mapreturn);
	m_http_manager.m_httpserver.send_data(request->m_clientfd, request->m_client_tag, package.m_str_respond);
	m_http_manager.m_httpserver.close_client(request->m_clientfd, request->m_client_tag);

}




