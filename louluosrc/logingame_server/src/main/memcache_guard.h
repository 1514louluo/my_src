#pragma once


#include "SDLogger.h"

#include "mycommon.h"

#include "memcache_client.h"

class memcache_guard
{
public:
	memcache_guard(void *param);
	~memcache_guard(void);

	int fetch_memcached_client_from_pool(void);

	memcache_client *get_memcache_client(void);

private:
	void *m_pparent;

	memcache_client *m_pclient;



};
