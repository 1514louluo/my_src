#include "memcache_client.h"

#include "SDConfigurationSingleton.h"

memcached_server_st *memcache_client::server = NULL;
time_t memcache_client::m_expiration = 0;

IMPL_LOGGER(memcache_client, logger);


memcache_client::memcache_client(void)
{
	m_memcached_st = NULL;
	if (server == NULL)
	{
		return;
	}
	m_memcached_st = memcached_create(NULL);;

	memcached_return rc;
	rc=memcached_server_push(m_memcached_st,server);
	if (rc != MEMCACHED_SUCCESS)
	{
		MY_LOGERROR("failed to get memcached client conn");
		memcached_free(m_memcached_st);
		m_memcached_st = NULL;
	}
}

memcache_client::~memcache_client(void)
{
	if (m_memcached_st != NULL)
	{
		memcached_free(m_memcached_st);
	}
}

void memcache_client::INIT_MEMCACHED_CLIENT_MODULE( void )
{
	memcached_return rc;
	if (server != NULL)
	{
		return;
	}

	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	std::string str_memcached_server_ip = config.getString("memcache_server_ip","127.0.0.1");
	int memcache_server_port = config.getInt("memcache_server_port", 2222);
	m_expiration = config.getInt("expiration", 0);
	server = memcached_server_list_append(NULL,str_memcached_server_ip.c_str(),memcache_server_port,&rc);
	
	if (server == NULL)
	{
		MY_LOGERROR("failed to create memcached erver node");
		return;
	}
	if (rc != MEMCACHED_SUCCESS)
	{
		MY_LOGERROR("failed to create memcached erver node");
		return;
	}

}
 
int memcache_client::set_value( std::string &str_key, std::string &str_value )
{
	memcached_return rc;
	if (m_memcached_st == NULL)
	{
		return -1;
	}

	time_t expiretime= time(NULL) + m_expiration;
	
	rc=memcached_set(m_memcached_st,str_key.c_str(),str_key.length(),str_value.c_str(),
		str_value.length(),expiretime,32);
	if(rc==MEMCACHED_SUCCESS)
	{
		//cout<<"Save data:"<<value<<" sucessful!"<<endl;
		return 0;
	}
	else
	{
		MY_LOGERROR("set memcache failed, key="<<str_key<<" value="<<str_value);
		return -1;
		//printf("save failed, value=%s\n", value.c_str());
	}
}

int memcache_client::set_value_with_expire( std::string &str_key, std::string &str_value, int expire )
{
	memcached_return rc;
	if (m_memcached_st == NULL)
	{
		return -1;
	}

	time_t expiretime;
	if (expire < 100)
	{
		expiretime = time(NULL) + m_expiration;
	} 
	else
	{
		expiretime = time(NULL) + expire;
	}
	

	rc=memcached_set(m_memcached_st,str_key.c_str(),str_key.length(),str_value.c_str(),
		str_value.length(),expiretime,32);
	if(rc==MEMCACHED_SUCCESS)
	{
		//cout<<"Save data:"<<value<<" sucessful!"<<endl;
		return 0;
	}
	else
	{
		MY_LOGERROR("set memcache failed, key="<<str_key<<" value="<<str_value);
		return -1;
		//printf("save failed, value=%s\n", value.c_str());
	}
}

int memcache_client::get_value( std::string &str_key, std::string &str_value )
{

	size_t value_length = 0;
	uint32_t flags=0;
	memcached_return rc;
	if (m_memcached_st == NULL)
	{
		return -1;
	}

	char* result = memcached_get(m_memcached_st,str_key.c_str(),str_key.length(),&value_length,&flags,&rc);
	
	
	
	if(rc == MEMCACHED_SUCCESS)
	{
		if (result == NULL)
		{
			MY_LOGERROR("fatal, memcached get NULL char point!!!"<<" key="<<str_key);
			return -1;
		}
		str_value.append(result, value_length);
		free(result);
		return 1;
		//printf("result=%s, length=%d\n",result, value_length);
	}
	else if (rc == MEMCACHED_NOTFOUND)
	{
		return 0;
	}
	else
	{
		MY_LOGERROR("memcache get failed, key="<<str_key);
		return -1;
		//printf("get failed, key=%s\n", key.c_str());
	}


}

int memcache_client::delete_value( std::string &str_key )
{

	size_t value_length = 0;
	uint32_t flags=0;
	memcached_return rc;
	if (m_memcached_st == NULL)
	{
		return -1;
	}
	rc = memcached_delete(m_memcached_st, str_key.c_str(), str_key.size(), 0);
	if (rc == MEMCACHED_SUCCESS)
	{
		return 1;
	}
	else if(rc == MEMCACHED_NOTFOUND)
	{
		MY_LOGDEBUG("delete memcache, key MEMCACHED_NOTFOUND, str_key="<<str_key);
		return 0;
	}
	else
	{
		MY_LOGERROR("failed to delete memcahced, key="<<str_key<<" rc="<<rc);
		return -1;
	}

}


