#include "redis_cli_guard.h"

#include "redis_pool.h"

redis_cli_guard::redis_cli_guard(void *pparent)
{
	m_pparent = pparent;
	m_pclient = NULL;
}

redis_cli_guard::~redis_cli_guard(void)
{
	if (m_pclient != NULL && m_pparent != NULL)
	{
		redis_pool *pool = (redis_pool *)m_pparent;
		pool->restore_client(m_pclient);

	}
}

int redis_cli_guard::fetch_redis_client_from_pool( void )
{
	if (m_pparent == NULL)
	{
		return -1;
	}
	redis_pool *pool = (redis_pool *)m_pparent;

	if (m_pclient != NULL)
	{
		// 已经从pool中取了一个出来，不能再取了
		return 0;
	}

	int result;
	result = pool->fetch_redis_client(&m_pclient);
	if (result == -1)
	{
		return -1;
	}
	else
	{
		return 0;
	}


}

redis_client * redis_cli_guard::get_client( void )
{

	return m_pclient;
}
