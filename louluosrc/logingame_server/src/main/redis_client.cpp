#include "redis_client.h"

std::string redis_client::m_redis_ip;
int         redis_client::m_redis_port;
int         redis_client::m_timeout_seconds;
int         redis_client::m_expire_seconds;

IMPL_LOGGER(redis_client, logger);
redis_client::redis_client(void)
{
	m_redis_context = NULL;
	timeval tv;
	tv.tv_sec = m_timeout_seconds;
	tv.tv_usec = 0;
	m_redis_context = redisConnectWithTimeout(m_redis_ip.c_str(),m_redis_port,tv);
	if (m_redis_context == NULL)
	{
		MY_LOGERROR("m_redis_context == NULL failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
	}
	if (m_redis_context->err != 0)
	{
		MY_LOGERROR("failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
	}
}


void redis_client::INIT_REDIS_CLIENT( void )
{

	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	m_redis_ip = config.getString("redis_server_ip","127.0.0.1");
	m_redis_port = config.getInt("redis_server_port", 6379);
	m_timeout_seconds = config.getInt("timeout_seconds", 10);
	m_expire_seconds = config.getInt("expire_seconds", 20000);

}


redis_client::~redis_client(void)
{
	if (m_redis_context != NULL)
	{
		redisFree(m_redis_context);
	}
}

int redis_client::set_value( std::string &str_key, std::string &str_value )
{
	int result;
	result = check_connection();
	if (result == -1)
	{
		return -1;
	}
	if (m_redis_context == NULL)
	{
		return -1;
	}
	 
	redisReply *reply= NULL;
	//std::string strexpirecmd;

	//strexpirecmd = strexpirecmd + REDIS_CMD_EXPIRE+str_key + "  " + SDUtility::format(m_expire_seconds);

	//int result = redisAppendCommand(m_redis_context,strcmd.c_str());
	result = redisAppendCommand(m_redis_context,"%s  %s  %s", REDIS_CMD_SET, str_key.c_str(), 
			 						str_value.c_str());
	if (result != 0)
	{
		std::string strcmd;
		strcmd = strcmd + REDIS_CMD_SET + str_key + "  "+ str_value;
		MY_LOGERROR("set cmd failed, cmd="<<strcmd<<"  m_redis_context->err="<<m_redis_context->err);
		return -1;
	}

	//MY_LOGDEBUG("expire cmd= "<<strexpirecmd);
	//result = redisAppendCommand(m_redis_context, strexpirecmd.c_str());
	/*result = redisAppendCommand(m_redis_context, "%s %s %s",REDIS_CMD_EXPIRE, str_key.c_str(),
					SDUtility::format(m_expire_seconds).c_str());
	if (result != 0)
	{
		MY_LOGERROR("set cmd failed, cmd="<<strexpirecmd<<"  m_redis_context->err="<<m_redis_context->err);
		return -1;
	}*/
	
	result = commit_cmd(&reply);
	if (result == -1)
	{
		std::string strcmd;
		strcmd = strcmd + REDIS_CMD_SET + str_key + "  "+ str_value;
		MY_LOGERROR(""<<strcmd<<"  failed");
		if (reply != NULL)
		{	
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}
	}
	if (reply == NULL)
	{
		return -1;
	}
	//// 需要处理缓冲区是否已经发送出去
	//if (is_cmd_send_out())
	//{
	//} 
	//else
	//{
	//	freeReplyObject(reply);
	//	void *ptemp=NULL;
	//	int result = redisGetReply(m_redis_context, &ptemp);
	//	if (result != 0)
	//	{
	//		MY_LOGERROR("redisGetReply(m_redis_context, &ptemp);  failed"
	//			<<"  m_redis_context->err="<<m_redis_context->err<<" ptemp="<<ptemp);
	//		return -1;
	//	}
	//	reply  =(redisReply *)ptemp;

	//}

	//if (reply == NULL)
	//{
	//	MY_LOGERROR("reply == NULL"<<"  m_redis_context->err="<<m_redis_context->err);	
	//	return -1;
	//}


	if (reply->type != REDIS_REPLY_STATUS)
	{
		MY_LOGERROR("redis_client::set_value return invalid type="<<reply->type<<" errstr="
					<<reply->str);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	if (reply->str == NULL || reply->len == 0)
	{
		MY_LOGERROR("redis_client::set_value reply->str == NULL !!!");
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	std::string str_reply;
	str_reply = reply->str;
	boost::trim(str_reply);
	if (str_reply != REDIS_REPLY_STATUS_OK)
	{
		MY_LOGERROR("redis_client::set_value reply not ok, staus="<<str_reply);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	//MY_LOGDEBUG("set key="<<str_key<<" value="<<str_value<<" finished");
	freeReplyObject(reply);
	reply = NULL;



	return 0;

}



int redis_client::get_value( std::string &str_key, std::string &str_value )
{

	int result;
	result = check_connection();
	if (result == -1)
	{
		return -1;
	}

	if (m_redis_context == NULL)
	{
		return -1;
	}
	
	redisReply *reply= NULL;
	result = redisAppendCommand(m_redis_context, "%s  %s",REDIS_CMD_GET,str_key.c_str());
	if (result != 0)
	{
		std::string strcmd;
		strcmd = REDIS_CMD_GET + str_key;
		MY_LOGERROR("redisAppendCommand cmd failed, cmd="<<strcmd<<"  m_redis_context->err="<<m_redis_context->err);
		return -1;
	}

	result = commit_cmd(&reply);
	if (result == -1)
	{
		std::string strcmd;
		strcmd = REDIS_CMD_GET + str_key;
		MY_LOGERROR("commit_cmd cmd failed, cmd="<<strcmd);
		if (reply != NULL)
		{	
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}
	}
	if (reply == NULL)
	{
		return -1;
	}

	//// 需要处理缓冲区是否已经发送出去
	//if (is_cmd_send_out())
	//{
	//} 
	//else
	//{
	//	freeReplyObject(reply);
	//	void *ptemp=NULL;
	//	int result = redisGetReply(m_redis_context, &ptemp);
	//	if (result != 0)
	//	{
	//		MY_LOGERROR("redisGetReply(m_redis_context, &ptemp);  failed");
	//		return -1;
	//	}
	//	reply  =(redisReply *)ptemp;
	//}

	//if (reply == NULL)
	//{
	//	return -1;
	//}

	if (reply->type != REDIS_REPLY_STRING)
	{
		if (reply->type == REDIS_REPLY_NIL)
		{
			MY_LOGERROR("reply->type == REDIS_REPLY_NIL");
			freeReplyObject(reply);
			reply = NULL;
			return 0;
		}
		else
		{
			MY_LOGERROR("redis_client::set_value return invalid type="<<reply->type<<" errstr="<<reply->str);
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}	
	}

	if (reply->str == NULL || reply->len == 0)
	{
		MY_LOGERROR("redis_client::get_value reply->str == NULL !!!");
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}

	str_value = reply->str;
	freeReplyObject(reply);
	reply = NULL;
	return 1;

	

}

bool redis_client::is_cmd_send_out( void )
{
	std::string str_outbuffer;
	str_outbuffer = m_redis_context->obuf;
	if (str_outbuffer.empty())
	{
		return true;
	} 
	else
	{
		return false;
	}

}

int redis_client::set_value_set( std::string &str_key, std::string &str_value )
{

	int result;
	result = check_connection();
	if (result == -1)
	{
		return -1;
	}
	if (m_redis_context == NULL)
	{
		return -1;
	}
	redisReply *reply= NULL;
	//std::string strexpirecmd;

	//strexpirecmd = strexpirecmd + REDIS_CMD_EXPIRE+str_key + "  " + SDUtility::format(m_expire_seconds);

	//int result = redisAppendCommand(m_redis_context,strcmd.c_str());
	result = redisAppendCommand(m_redis_context,"%s  %s  %s",REDIS_CMD_SADD, str_key.c_str(), str_value.c_str());
	if (result != 0)
	{
		MY_LOGERROR("set cmd failed, cmd="<<REDIS_CMD_SADD<<" "<<str_key<<" "<<str_value);
		return -1;
	}

	//MY_LOGDEBUG("expire cmd= "<<strexpirecmd);
	/*result = redisAppendCommand(m_redis_context, strexpirecmd.c_str());
	if (result == -1)
	{
		return -1;
	}*/

	result = commit_cmd(&reply);

	if (result == -1)
	{
		if (reply != NULL)
		{	
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}
	}
	if (reply == NULL)
	{
		return -1;
	}



	if (reply->type != REDIS_REPLY_INTEGER)
	{
		MY_LOGERROR("redis_client::set_value_set return invalid type="<<reply->type);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	//MY_LOGDEBUG("redis SADD validation, rtn value="<<reply->integer);
	MY_LOGDEBUG("set key="<<str_key<<" value="<<str_value<<" finished");
	freeReplyObject(reply);
	reply = NULL;
	return 0;

}

int redis_client::get_value_set( std::string &str_key, std::vector<std::string> &vec )
{

	int result;
	result = check_connection();
	if (result == -1)
	{
		return -1;
	}
	if (m_redis_context == NULL)
	{
		return -1;
	}
	std::string strcmd;
	strcmd = REDIS_CMD_SMEMBER + str_key;
	redisReply *reply= NULL;
	result = redisAppendCommand(m_redis_context, "%s  %s", REDIS_CMD_SMEMBER,str_key.c_str());

	if (result != 0)
	{
		return -1;
	}

	// 需要处理缓冲区是否已经发送出去
	result = commit_cmd(&reply);

	if (result == -1)
	{
		if (reply != NULL)
		{	
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}
	}
	if (reply == NULL)
	{
		return -1;
	}


	if (reply->type != REDIS_REPLY_ARRAY)
	{
		if (reply->type == REDIS_REPLY_ERROR)
		{
			MY_LOGERROR("redis_client::get_value_set  reply->type != REDIS_REPLY_ARRAY, reply->type="
				<<reply->type<<" errstr="<<reply->str);
		} 
		else
		{
			MY_LOGERROR("redis_client::get_value_set  reply->type != REDIS_REPLY_ARRAY, reply->type="<<reply->type);
		}
		
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	if (reply->elements == 0)
	{
		freeReplyObject(reply);
		reply = NULL;
		return 0;

	}

	for (size_t i=0;i<reply->elements; i++)
	{
		redisReply *replyvalue = reply->element[i];
		if (replyvalue == NULL)
		{
			freeReplyObject(reply);
			reply = NULL;
			MY_LOGERROR("replyvalue == NULL, drop this redisReply element");
			continue;
			//return -1;
		}
		if (replyvalue->str == NULL)
		{
			freeReplyObject(reply);
			reply = NULL;
			MY_LOGERROR("replyvalue->str == NULL, drop this redisReply element");
			continue;
			//return -1;
		}
		std::string str_value;
		str_value = replyvalue->str;
		vec.push_back(str_value);
	}
	freeReplyObject(reply);
	reply = NULL;
	return vec.size();


}

int redis_client::del_value_set( std::string &str_key, std::string &str_value )
{
	int result;
	result = check_connection();
	if (result == -1)
	{
		return -1;
	}
	if (m_redis_context == NULL)
	{
		return -1;
	}
	//std::string strcmd;
	//strcmd = strcmd + REDIS_CMD_SREM + str_key + "  "+ str_value;
	redisReply *reply= NULL;
	
	//MY_LOGDEBUG("expire cmd= "<<strexpirecmd);
	result = redisAppendCommand(m_redis_context, "%s %s  %s",REDIS_CMD_SREM, str_key.c_str(), str_value.c_str());
	if (result == -1)
	{
		return -1;
	} 
	result = commit_cmd(&reply);

	if (result == -1)
	{
		MY_LOGERROR(""<<REDIS_CMD_SREM<<" "<<str_key<<" "<<str_value<<" failed");
		if (reply != NULL)
		{	
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}
	}
	if (reply == NULL)
	{
		return -1;
	}



	/*if (reply->type != REDIS_REPLY_INTEGER)
	{
		MY_LOGERROR("redis_client::set_value_set return invalid type="<<reply->type);
		freeReplyObject(reply);
		return -1;
	}*/
	//MY_LOGDEBUG("redis SADD validation, rtn value="<<reply->integer);
	MY_LOGDEBUG("SREM  del  set key="<<str_key<<" value="<<str_value<<" finished");
	freeReplyObject(reply);
	reply = NULL;
	return 0;

}

int redis_client::commit_cmd( redisReply **ppreply )
{
	if (m_redis_context == NULL)
	{
		MY_LOGERROR("m_redis_context == NULL");
		*ppreply = NULL;
		return -1;
	}
	// 先检查连接是否有效，如果无效，需要做重连操作
	if (m_redis_context->err != 0)
	{
		MY_LOGERROR("redis clent lose connection to server, errstr="<<m_redis_context->errstr<<" ready to reconnect");
		std::string str_outbuffer;
		str_outbuffer = m_redis_context->obuf;
		redisFree(m_redis_context);
		timeval tv;
		tv.tv_sec = m_timeout_seconds;
		tv.tv_usec = 0;
		m_redis_context = redisConnectWithTimeout(m_redis_ip.c_str(),m_redis_port,tv);
		if (m_redis_context == NULL)
		{
			MY_LOGERROR("reconnect m_redis_context == NULL failed to connect to redis server,ip="
						<<m_redis_ip<<" port="<<m_redis_port);
			*ppreply = NULL;
			return -1;
		}

		if (m_redis_context->err != 0)
		{
			*ppreply = NULL;
			MY_LOGERROR("failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}

		// 将对应的请求重新添加上去
		int result1 = redisAppendCommand(m_redis_context,str_outbuffer.c_str());
		if (result1 != 0)
		{
			MY_LOGERROR("reconnection, set cmd failed, cmd="<<str_outbuffer);
			return -1;
		}

	}

	// 连接正常，开始数据收发
	// 需要处理缓冲区是否已经发送出去
	if (is_cmd_send_out())
	{
	} 
	else
	{
		//freeReplyObject(reply);
		//void *ptemp=NULL;
		*ppreply = NULL;
		int result = redisGetReply(m_redis_context, (void **)ppreply);
		if (result != 0)
		{
			MY_LOGERROR("redisGetReply(m_redis_context, ppreply);  failed"
				<<"  m_redis_context->err="<<m_redis_context->err<<" ptemp="<<*ppreply);
			return -1;
		}
	}
	if (*ppreply == NULL)
	{
		return -1;
	}
	return 0;

}

int redis_client::check_connection( void )
{
	if (m_redis_context == NULL)
	{
		MY_LOGERROR("m_redis_context == NULL ready to create m_redis_context");
		timeval tv;
		tv.tv_sec = m_timeout_seconds;
		tv.tv_usec = 0;
		m_redis_context = redisConnectWithTimeout(m_redis_ip.c_str(),m_redis_port,tv);
		if (m_redis_context == NULL)
		{
			MY_LOGERROR("m_redis_context == NULL failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}
		if (m_redis_context->err != 0)
		{
			MY_LOGERROR("failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}
	}

	if (m_redis_context->err != 0)  // 网络出现故障，需要做重连操作，已有请求数据全部丢弃
	{	
		MY_LOGERROR("check_connection redis network error, m_redis_context->err="<<m_redis_context->err<<" errstr="<<
					m_redis_context->errstr<<" ready to reconnect server");
		redisFree(m_redis_context);
		timeval tv;
		tv.tv_sec = m_timeout_seconds;
		tv.tv_usec = 0;
		m_redis_context = redisConnectWithTimeout(m_redis_ip.c_str(),m_redis_port,tv);
		if (m_redis_context == NULL)
		{
			MY_LOGERROR("m_redis_context == NULL failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}
		if (m_redis_context->err != 0)
		{
			MY_LOGERROR("failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}

	}
	return 0;


}

int redis_client::set_expire( std::string &str_key,int expire_second/*=0*/ )
{

	int result;
	result = check_connection();
	if (result == -1)
	{
		return -1;
	}

	if (m_redis_context == NULL)
	{
		return -1;
	}
	
	if (expire_second == 0)
	{
		expire_second = m_expire_seconds;
	}
	result = redisAppendCommand(m_redis_context, "%s %s %s",REDIS_CMD_EXPIRE, str_key.c_str(),
					SDUtility::format(expire_second).c_str());
	if (result != 0)
	{
		MY_LOGERROR("set cmd failed, cmd="<<REDIS_CMD_EXPIRE<<"  m_redis_context->err="<<m_redis_context->err);
		return -1;
	}

	if (result != 0)
	{
		return -1;
	}

	redisReply *reply=NULL;

	// 需要处理缓冲区是否已经发送出去
	result = commit_cmd(&reply);

	if (result == -1)
	{
		if (reply != NULL)
		{	
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}
	}
	if (reply == NULL)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_INTEGER)
	{
		MY_LOGERROR("redis set expire failed, reply->type != REDIS_REPLY_INTEGER, type="<<reply->type);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	int valid = reply->integer;
	

	freeReplyObject(reply);
	reply = NULL;
	return valid;

	
}


