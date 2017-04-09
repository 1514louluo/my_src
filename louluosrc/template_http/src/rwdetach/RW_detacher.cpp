#include "RW_detacher.h"
IMPL_LOGGER(RW_detacher, logger);	

redis_pool  RW_detacher::m_redis_rpool;
redis_pool  RW_detacher::m_redis_wpool;
mysql_pool RW_detacher::m_mysql_rpool;
mysql_pool RW_detacher::m_mysql_wpool;
RW_detacher::RW_detacher(){}
RW_detacher::~RW_detacher(){}
int RW_detacher::init_RW_detacher(int inittype)
{
	int result;
	switch(inittype)
	{
		case RW_DETACHER_ALL:
		{
			result = init_RW_datacher_for_mysql();
			if(result)
			{
				SYS_LOG_ERROR("<!!!>failed to init_RW_datacher_for_mysql! ");
				return -1;
			}
			result = init_RW_datacher_for_redis();
			if(result)
			{
				SYS_LOG_ERROR("<!!!>failed to init_RW_datacher_for_redis! ");
				return -1;
			}
			break;
		}
		case RW_DETACHER_ONLY_MYSQL:
		{
			result = init_RW_datacher_for_mysql();
			if(result)
			{
				SYS_LOG_ERROR("<!!!>failed to init_RW_datacher_for_mysql! ");
				return -1;
			}
			break;
		}
		case RW_DETACHER_ONLY_REDIS:
		{
			result = init_RW_datacher_for_redis();
			if(result)
			{
				SYS_LOG_ERROR("<!!!>failed to init_RW_datacher_for_redis! ");
				return -1;
			}
			break;
		}
		default:
		{
			SYS_LOG_ERROR("<!!!>unknow type in init_RW_datacher! ");
			return -1;
		}
	}
	
	return 0;
}
/*<======================for redis================================>*/
int RW_detacher::init_RW_datacher_for_redis(void)
{
	int result;
	const configref& config = config_singleton::get_instance()->get_config();
/*init redis config*/
	int redis_hold_timeout = config.getInt("redis_hold_timeout", 100);
	int redis_rpool_size = config.getInt("redis_rpool_size", 10);
	string_type redis_rhost = config.getString("redis_rhost", "127.0.0.1");
	uint16_t redis_rport = (uint16_t)config.getInt("redis_rport", 6379);
	int redis_rtimeout = config.getInt("redis_rtimeout", 10);
	int redis_rexpire = config.getInt("redis_rexpire", 20000);

	int redis_wpool_size = config.getInt("redis_wpool_size", 10);
	string_type redis_whost = config.getString("redis_whost", "127.0.0.1");
	uint16_t redis_wport = (uint16_t)config.getInt("redis_wport", 6379);
	int redis_wtimeout = config.getInt("redis_wtimeout", 10);
	int redis_wexpire = config.getInt("redis_wexpire", 20000);

	result = m_redis_rpool.init_pool(redis_rpool_size,  redis_hold_timeout, redis_rhost, redis_rport, redis_rtimeout, redis_rexpire);
	if(result)
	{
		SYS_LOG_ERROR("<!!!>failed to init redis_pool for read! ");
		return -1;
	}
	result = m_redis_wpool.init_pool(redis_wpool_size,  redis_hold_timeout, redis_whost, redis_wport, redis_wtimeout, redis_wexpire);
	if(result)
	{
		SYS_LOG_ERROR("<!!!>failed to init redis_pool for write! ");
		return -1;
	}
	return 0;
}
redis_client *RW_detacher::get_redis_reader(void)
{
	return m_redis_rpool.fetch_single();
}
redis_client *RW_detacher::get_redis_writer(void)
{
	return m_redis_wpool.fetch_single();
}
int RW_detacher::reback_redis_reader(redis_client *pclient)
{
	return m_redis_rpool.reback_single( pclient);
}
int RW_detacher::reback_redis_writer(redis_client *pclient)
{
	return m_redis_wpool.reback_single( pclient);
}

int RW_detacher::do_redis_hget(const string_type &key,const string_type &field, string_type &value)
{
	int rt;
	redis_client *pclient = get_redis_reader();
	if (NULL == pclient)    return -1;
	rt = pclient->hget(key, field, value);
	reback_redis_reader(pclient);
	return rt;
}
int RW_detacher::do_redis_hgetall( const string_type & key, string_pair_vector & out )
{
	int rt;
	redis_client *pclient = get_redis_reader();
	if (NULL == pclient)    return -1;
	rt = pclient->hgetall(key,  out);
	reback_redis_reader(pclient);
	return rt;
}
int RW_detacher::do_redis_hset( const string_type & key, const string_type & field, const string_type & value )
{
	int rt;
	redis_client *pclient = get_redis_writer();
	if (NULL == pclient)    return -1;
	rt = pclient->hset(key, field, value);
	reback_redis_writer(pclient);
	return rt;
}
int RW_detacher::do_redis_hdel( const string_type & key, const string_type & field )
{
	int rt;
	redis_client *pclient = get_redis_writer();
	if (NULL == pclient)    return -1;
	rt = pclient->hdel(key, field);
	reback_redis_writer(pclient);
	return rt;
}

int RW_detacher::do_redis_set(const string_type & key, const string_type & value)
{
	int rt;
	redis_client *pclient = get_redis_writer();
	if (NULL == pclient)    return -1;
	rt = pclient->set(key, value);
	reback_redis_writer(pclient);
	return rt;
}

int RW_detacher::do_redis_setex(const string_type & key, const string_type & value, unsigned int secs)
{
	int rt;
	redis_client *pclient = get_redis_writer();
	if (NULL == pclient)    return -1;
	rt = pclient->setex(key, value, secs);
	reback_redis_writer(pclient);
	return rt;
}

int RW_detacher::do_redis_get(const string_type & key, string_type &value)
{
	int rt;
	redis_client *pclient = get_redis_reader();
	if (NULL == pclient)    return -1;
	rt = pclient->get(key, value);
	reback_redis_reader(pclient);
	return rt;
}
int RW_detacher::do_redis_del(const string_type & key)
{
	int rt;
	redis_client *pclient = get_redis_writer();
	if (NULL == pclient)    return -1;
	rt = pclient->del(key);
	reback_redis_writer(pclient);
	return rt;
}
/*<======================for mysql================================>*/
int RW_detacher::init_RW_datacher_for_mysql(void)
{
	int result;
	const configref& config = config_singleton::get_instance()->get_config();
/*init mysql config*/
	int mysql_hold_timeout=config.getInt("mysql_hold_timeout", 100);

	int mysql_rpool_size = config.getInt("mysql_rpool_size", 10);
	string_type mysql_rhost=config.getString("mysql_rhost", "127.0.0.1");
	string_type mysql_ruser=config.getString("mysql_ruser", "root");
	string_type mysql_rpasswd=config.getString("mysql_rpasswd", "111");
	uint16_t mysql_rport =(uint16_t)config.getInt("mysql_rport", 3306);;
	string_type mysql_rdbname=config.getString("mysql_rdbname", "xxx");
//	string_type mysql_rszSocket=config.getString("mysql_rszSocket", "");
	unsigned long mysql_riCientFlag=(unsigned long)config.getInt("mysql_riCientFlag", 0);
	
	int mysql_wpool_size=config.getInt("mysql_wpool_size", 10);
	string_type mysql_whost=config.getString("mysql_whost", "127.0.0.1");
	string_type mysql_wuser=config.getString("mysql_wuser", "root");
	string_type mysql_wpasswd=config.getString("mysql_wpasswd", "111");
	uint16_t mysql_wport =(uint16_t)config.getInt("mysql_wport", 3306);;
	string_type mysql_wdbname=config.getString("mysql_wdbname", "xxx");
//	string_type mysql_wszSocket=config.getString("mysql_wszSocket", "");
	unsigned long mysql_wiCientFlag=(unsigned long)config.getInt("mysql_wiCientFlag", 0);

	result = m_mysql_rpool.init_pool(mysql_rpool_size,  mysql_hold_timeout, mysql_rhost.c_str(),
							       mysql_rport, mysql_ruser.c_str(), mysql_rpasswd.c_str(),
							       mysql_rdbname.c_str(), NULL, mysql_riCientFlag);
	if(result)
	{
		SYS_LOG_ERROR("<!!!>failed to init mysql_pool for read! ");
		return -1;
	}
	result = m_mysql_wpool.init_pool(mysql_wpool_size,  mysql_hold_timeout, mysql_whost.c_str(),
							       mysql_wport, mysql_wuser.c_str(), mysql_wpasswd.c_str(),
							       mysql_wdbname.c_str(), NULL, mysql_wiCientFlag);
	if(result)
	{
		SYS_LOG_ERROR("<!!!>failed to init mysql_pool for write! ");
		return -1;
	}
	return 0;
}
CMysql *RW_detacher::get_mysql_reader(void)
{
	return m_mysql_rpool.fetch_single();
}
CMysql *RW_detacher::get_mysql_writer(void)
{
	return m_mysql_wpool.fetch_single();
}
int RW_detacher::reback_mysql_reader(CMysql *pclient)
{
	return m_mysql_rpool.reback_single(pclient);
}
int RW_detacher::reback_mysql_writer(CMysql *pclient)
{
	return m_mysql_wpool.reback_single(pclient);
}

int RW_detacher::do_mysql_opt_write(string_type &sql)
{
	CMysql *pclient = NULL;
	pclient = get_mysql_writer();
	if (NULL == pclient)    return -1;
	
	try{
		pclient->Query(sql);
	}
	catch(CMysqlException& e)
	{
		SYS_LOG_ERROR("exec sql failed, sql="<<sql<<" error="<<e.GetErrMsg());
		return -1;
	}
	reback_mysql_writer(pclient);

	return 0;
	
}

int RW_detacher::do_mysql_opt_read(string_type &sql,  string_vector &reply)
{
	CMysql *pclient = NULL;
	pclient = get_mysql_reader();
	if (NULL == pclient)    return -1;

	try{
		pclient->Query(sql);
		try
		{
			pclient->StoreResult();
		}
		catch(CMysqlException& e)
		{
			SYS_LOG_ERROR("exec sql failed, sql="<<sql<<" error="<<e.GetErrMsg());
			return -1;
		}
	}
	catch(CMysqlException& e)
	{
		SYS_LOG_ERROR("exec sql failed, sql="<<sql<<" error="<<e.GetErrMsg());
		return -1;
	}

  	while(pclient->FetchRow())
    	{
    		string_type json_reply;
		cJSON *pjsonobj = cJSON_CreateObject();
    		int field_index = 0;
		for(; field_index < pclient->GetAffectedCols(); ++field_index)
		{
			string_type key = pclient->GetFieldName(field_index);
			string_type value = pclient->GetField(field_index);
			cJSON_AddStringToObject(pjsonobj, key.c_str(), value.c_str());
		}
		char *pdata = cJSON_PrintUnformatted(pjsonobj);
		json_reply = pdata;
		reply.push_back(json_reply);
		free(pdata);
		cJSON_Delete(pjsonobj);
    	}

	reback_mysql_reader(pclient);

	return 0;
}


