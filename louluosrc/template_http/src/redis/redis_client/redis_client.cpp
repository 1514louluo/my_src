#include "redis_client.h"

IMPL_LOGGER(redis_client, logger);
redis_client::redis_client(const string_type &ip, int port, int timeout, int expire):m_redis_ip(ip),m_redis_port(port),m_timeout_seconds(timeout),m_expire_seconds(expire)
{
	m_redis_context = NULL;
}

redis_client::~redis_client(void)
{
	if (m_redis_context != NULL)
	{
		redisFree(m_redis_context);
	}
}

/*
int redis_client::pipeline_wtcmd(const string_pair_vector &cmd, string_pair_vector &out)
{
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}
	redisReply *reply= NULL;
	BOOST_FOREACH(const string_pair &each_cmd, cmd)
	{
		string_type hcmd = each_cmd->first;		
		string_type ecmd = each_cmd->second;		
		result = redisAppendCommand(m_redis_context,"%s %s",hcmd.c_str(), ecmd.c_str());
		if (result != 0)
		{
			SYS_LOG_ERROR("set cmd failed, cmd="<<hcmd<<" "<<ecmd);
			return -1;
		}
	}
	
	result = commit_cmd(&reply);
	if (result == -1)
	{
		SYS_LOG_ERROR("pipeline commit cmd failed!!");
		if (reply != NULL)
		{	
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}
	}
	
	BOOST_FOREACH(const string_pair &each_cmd, cmd)
	{
		if(redisGetReply(context,&reply) != REDIS_OK)
		{
			out.push_back(each_cmd);
			SYS_LOG_ERROR("pipeline<====>:cmd failed, cmd="<<each_cmd->first<<" "<<each_cmd->second);
		}
		else
		{
			if (reply->type != REDIS_REPLY_STATUS || reply->type != REDIS_REPLY_INTEGER)
			{
				out.push_back(each_cmd);
			}
		}
    		freeReplyObject(reply);
		reply = NULL;
	}
	
}*/

int redis_client::set(const string_type &key, const string_type &value)
{
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}
	 
	redisReply *reply = (redisReply *)redisCommand(m_redis_context,"%s %s %s", REDIS_CMD_SET, key.c_str(), 
			 						value.c_str());
	if (NULL == reply)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_STATUS)
	{
		SYS_LOG_ERROR("redis_client::set return invalid type="<<reply->type<<"cmd = "<<REDIS_CMD_SET<<" "<<key<<" "<<value);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	if (reply->str == NULL || reply->len == 0)
	{
		SYS_LOG_ERROR("redis_client::set reply->str == NULL !!!");
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}

	if (strcasecmp(reply->str,REDIS_REPLY_STATUS_OK))
	{
		SYS_LOG_ERROR("redis_client::set reply not ok, staus="<<reply->str);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	
	freeReplyObject(reply);
	reply = NULL;

	return 0;
}

int redis_client::setex(const string_type &key, const string_type &value, unsigned int secs)
{
	if(0 > secs)
	{
		SYS_LOG_ERROR("secs < 0!!");
		return -1;
	}
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}
	 
	redisReply *reply = (redisReply *)redisCommand(m_redis_context,"%s %s %s %s", REDIS_CMD_SETEX, key.c_str(), 
			 						SDUtility::format(secs).c_str(), value.c_str());
	if (NULL == reply)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_STATUS)
	{
		SYS_LOG_ERROR("redis_client::set return invalid type="<<reply->type<<"cmd = "<<REDIS_CMD_SETEX<<" "<<key<<" "<<secs<<" "<<value);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	if (reply->str == NULL || reply->len == 0)
	{
		SYS_LOG_ERROR("redis_client::set reply->str == NULL !!!");
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}

	if (strcasecmp(reply->str,REDIS_REPLY_STATUS_OK))
	{
		SYS_LOG_ERROR("redis_client::set reply not ok, staus="<<reply->str);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}
	
	freeReplyObject(reply);
	reply = NULL;

	return 0;
}

int redis_client::get(const string_type &key, string_type &value)
{
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}

	redisReply *reply = (redisReply *)redisCommand(m_redis_context,"%s %s", REDIS_CMD_GET, key.c_str());
	if (NULL == reply)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_STRING)
	{
		if (reply->type == REDIS_REPLY_NIL)
		{
			SYS_LOG_ERROR("reply->type == REDIS_REPLY_NIL");
			freeReplyObject(reply);
			reply = NULL;
			return 0;
		}
		else
		{
			SYS_LOG_ERROR("redis_client::get return invalid type="<<reply->type<<" errstr=" <<reply->str<<"cmd = "<<REDIS_CMD_GET<<" "<<key);
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}	
	}

	if (reply->str == NULL || reply->len == 0)
	{
		SYS_LOG_ERROR("redis_client::get reply->str == NULL !!!");
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}

	value = reply->str;	
	freeReplyObject(reply);
	reply = NULL;

	return 0;
}

int redis_client::del(const string_type &key)
{
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}

	redisReply *reply = (redisReply *)redisCommand(m_redis_context,"%s %s", REDIS_CMD_DEL, key.c_str());
	if (NULL == reply)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_INTEGER)
	{
		SYS_LOG_ERROR("redis_client::del return invalid type="<<reply->type<<"cmd = "<<REDIS_CMD_DEL<<" "<<key);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}

	result = reply->integer;
	freeReplyObject(reply);
	reply = NULL;
	return result;
}

int redis_client::expire(const string_type & key, unsigned int secs)
{
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}

	redisReply *reply = (redisReply *)redisCommand(m_redis_context,"%s %s %s", REDIS_CMD_EXPIRE, key.c_str(),
								SDUtility::format(secs).c_str());
	if (NULL == reply)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_INTEGER)
	{
		SYS_LOG_ERROR("redis_client::expire return invalid type="<<reply->type<<"cmd = "<<REDIS_CMD_EXPIRE<<" "<<key<<" "<<secs);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}

	result = reply->integer;
	freeReplyObject(reply);
	reply = NULL;
	return result;
}
	
int redis_client::hget(const string_type &key,const string_type &field, string_type &value)
{
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}

	redisReply *reply = (redisReply *)redisCommand(m_redis_context,"%s %s %s", REDIS_CMD_HGET, key.c_str(),
								field.c_str());
	if (NULL == reply)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_STRING)
	{
		if (reply->type == REDIS_REPLY_NIL)
		{
			SYS_LOG_ERROR("reply->type == REDIS_REPLY_NIL");
			freeReplyObject(reply);
			reply = NULL;
			return 0;
		}
		else
		{
			SYS_LOG_ERROR("redis_client::hget return invalid type="<<reply->type<<" errstr=" <<reply->str
						<<"cmd = "<<REDIS_CMD_HGET<<" "<<key<<" "<<field);
			freeReplyObject(reply);
			reply = NULL;
			return -1;
		}	
	}

	if (reply->str == NULL || reply->len == 0)
	{
		SYS_LOG_ERROR("redis_client::hget reply->str == NULL !!!");
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}

	value = reply->str;
	freeReplyObject(reply);
	reply = NULL;
	return 0;
}

int redis_client::hgetall(const string_type &key, string_pair_vector &out)
{
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}

	redisReply *reply = (redisReply *)redisCommand(m_redis_context,"%s %s", REDIS_CMD_HGETALL, key.c_str());
	if (NULL == reply)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_ARRAY)
	{
		if (reply->type == REDIS_REPLY_ERROR)
		{
			SYS_LOG_ERROR("redis_client::hgetall reply->type != REDIS_REPLY_ARRAY, reply->type="
				<<reply->type<<" errstr="<<reply->str);
		} 
		else
		{
			SYS_LOG_ERROR("redis_client::hgetall reply->type != REDIS_REPLY_ARRAY, reply->type="<<reply->type);
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

	for (size_t i=0;i<reply->elements; i+=2)
	{
		redisReply *reply_key = reply->element[i];
		redisReply *reply_value = reply->element[i+1];
		if (reply_key == NULL || reply_value == NULL)
		{
			SYS_LOG_ERROR("replyvalue == NULL, drop this redisReply element");
			continue;
		}
		if (reply_key->str == NULL || reply_value->str == NULL)
		{
			SYS_LOG_ERROR("replyvalue->str == NULL, drop this redisReply element");
			continue;
		}
		string_type str_key = reply_key->str;
		string_type str_value = reply_value->str;
		out.push_back(make_pair(str_key, str_value));
	}
	
	freeReplyObject(reply);
	reply = NULL;
	return 0;
}

int redis_client::hset(const string_type &key, const string_type &field, const string_type &value)
{
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}

	redisReply *reply = (redisReply *)redisCommand(m_redis_context,"%s %s %s %s", REDIS_CMD_HSET, key.c_str(),
								field.c_str(), value.c_str());
	if (NULL == reply)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_INTEGER)
	{
		SYS_LOG_ERROR("redis_client::hset return invalid type="<<reply->type<<" errstr=" <<reply->str
				<<"cmd = "<<REDIS_CMD_HSET<<" "<<key<<" "<<field<<" "<<value);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}

	result = reply->integer;
	freeReplyObject(reply);
	reply = NULL;
	return result;
}

int redis_client::hdel(const string_type &key, const string_type &field)
{
	int result;
	result = check_connection();
	if (result == -1 || NULL == m_redis_context)
	{
		return -1;
	}

	redisReply *reply = (redisReply *)redisCommand(m_redis_context,"%s %s %s", REDIS_CMD_HDEL, key.c_str(),
								field.c_str());
	if (NULL == reply)
	{
		return -1;
	}

	if (reply->type != REDIS_REPLY_INTEGER)
	{
		SYS_LOG_ERROR("redis_client::hdel return invalid type="<<reply->type<<" errstr=" <<reply->str
				<<"cmd = "<<REDIS_CMD_HDEL<<" "<<key<<" "<<field);
		freeReplyObject(reply);
		reply = NULL;
		return -1;
	}

	result = reply->integer;
	freeReplyObject(reply);
	reply = NULL;
	return result;
}

int redis_client::connect(void)
{
	timeval tv;
	tv.tv_sec = m_timeout_seconds;
	tv.tv_usec = 0;
	m_redis_context = redisConnectWithTimeout(m_redis_ip.c_str(),m_redis_port,tv);
	if (m_redis_context == NULL)
	{
		SYS_LOG_ERROR("m_redis_context == NULL failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
		return -1;
	}
	if (m_redis_context->err != 0)
	{
		SYS_LOG_ERROR("failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
		return -1;
	}
	return 0;
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

int redis_client::commit_cmd( redisReply **ppreply )
{
	if (m_redis_context == NULL)
	{
		SYS_LOG_ERROR("m_redis_context == NULL");
		*ppreply = NULL;
		return -1;
	}
	// �ȼ�������Ƿ���Ч�������Ч����Ҫ����������
	if (m_redis_context->err != 0)
	{
		SYS_LOG_ERROR("redis clent lose connection to server, errstr="<<m_redis_context->errstr<<" ready to reconnect");
		std::string str_outbuffer;
		str_outbuffer = m_redis_context->obuf;
		redisFree(m_redis_context);
		timeval tv;
		tv.tv_sec = m_timeout_seconds;
		tv.tv_usec = 0;
		m_redis_context = redisConnectWithTimeout(m_redis_ip.c_str(),m_redis_port,tv);
		if (m_redis_context == NULL)
		{
			SYS_LOG_ERROR("reconnect m_redis_context == NULL failed to connect to redis server,ip="
						<<m_redis_ip<<" port="<<m_redis_port);
			*ppreply = NULL;
			return -1;
		}

		if (m_redis_context->err != 0)
		{
			*ppreply = NULL;
			SYS_LOG_ERROR("failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}

		// ����Ӧ���������������ȥ
		int result1 = redisAppendCommand(m_redis_context,str_outbuffer.c_str());
		if (result1 != 0)
		{
			SYS_LOG_ERROR("reconnection, set cmd failed, cmd="<<str_outbuffer);
			return -1;
		}

	}

/*	// ������������ʼ�����շ�
	// ��Ҫ���������Ƿ��Ѿ����ͳ�ȥ
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
			SYS_LOG_ERROR("redisGetReply(m_redis_context, ppreply);  failed"
				<<"  m_redis_context->err="<<m_redis_context->err<<" ptemp="<<*ppreply);
			return -1;
		}
	}
	if (*ppreply == NULL)
	{
		return -1;
	}
*/
	return 0;

}

int redis_client::check_connection( void )
{
	if (m_redis_context == NULL)
	{
		SYS_LOG_ERROR("m_redis_context == NULL ready to create m_redis_context");
		timeval tv;
		tv.tv_sec = m_timeout_seconds;
		tv.tv_usec = 0;
		m_redis_context = redisConnectWithTimeout(m_redis_ip.c_str(),m_redis_port,tv);
		if (m_redis_context == NULL)
		{
			SYS_LOG_ERROR("m_redis_context == NULL failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}
		if (m_redis_context->err != 0)
		{
			SYS_LOG_ERROR("failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}
	}

	if (m_redis_context->err != 0)  // ������ֹ��ϣ���Ҫ������������������������ȫ������
	{	
		SYS_LOG_ERROR("check_connection redis network error, m_redis_context->err="<<m_redis_context->err<<" errstr="<<
					m_redis_context->errstr<<" ready to reconnect server");
		redisFree(m_redis_context);
		timeval tv;
		tv.tv_sec = m_timeout_seconds;
		tv.tv_usec = 0;
		m_redis_context = redisConnectWithTimeout(m_redis_ip.c_str(),m_redis_port,tv);
		if (m_redis_context == NULL)
		{
			SYS_LOG_ERROR("m_redis_context == NULL failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}
		if (m_redis_context->err != 0)
		{
			SYS_LOG_ERROR("failed to connect to redis server,ip="<<m_redis_ip<<" port="<<m_redis_port);
			return -1;
		}

	}
	return 0;

}


	
