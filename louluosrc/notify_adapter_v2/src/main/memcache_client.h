#pragma once


#include <libmemcached/memcached.h>
#include "SDLogger.h"
#include "mycommon.h"

class memcache_client
{
public:
	memcache_client(void);
	~memcache_client(void);

	int set_value(std::string &str_key, std::string &str_value);

	int get_value(std::string &str_key, std::string &str_value);

	int delete_value(std::string &str_key);

	int set_value_with_expire( std::string &str_key, std::string &str_value, int expire );


	static void INIT_MEMCACHED_CLIENT_MODULE(void);

private:
	static memcached_server_st *server;
	static time_t m_expiration;
	memcached_st *m_memcached_st;


	DECL_LOGGER(logger);
};
