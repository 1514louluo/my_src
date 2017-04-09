#include "memcache_guard.h"


#include "memcache_cli_pool.h"

memcache_guard::memcache_guard(void *param)
{
	m_pparent = param;
	m_pclient = NULL;
}

memcache_guard::~memcache_guard(void)
{
	if (m_pclient != NULL  && m_pparent != NULL)
	{
		memcache_cli_pool *pool = (memcache_cli_pool *)m_pparent;
		pool->restore_client(m_pclient);
		m_pclient = NULL;
	}
}

int memcache_guard::fetch_memcached_client_from_pool( void )
{
	if (m_pparent == NULL)
	{
		return -1;
	}

	memcache_cli_pool *pool = (memcache_cli_pool *)m_pparent;
	
	int result = pool->fetch_client(&m_pclient);
	if (result == 0)
	{
		return 0;
	}
	else
	{
		m_pclient = NULL;
		return -1;
	}

}

memcache_client * memcache_guard::get_memcache_client( void )
{
	return m_pclient;

}
