#pragma once

#include "SDLogger.h"
#include "block_queue.h"
#include "mycommon.h"
#include "memcache_client.h"
#include "SDUtility.h"
#include "global_var.h"
class memcache_cli_pool
{
public:
	memcache_cli_pool(void);
	~memcache_cli_pool(void);

	void init_memcache_cli_pool( void );

	int fetch_client(memcache_client **ppclient);

	int restore_client(memcache_client *pclient);


private:
	DECL_LOGGER(logger);
	block_queue< memcache_client * > m_client_queue;

	uint32_t m_memcache_pool_fetch_timeout_threashold;

	int m_pool_low_water;
};
