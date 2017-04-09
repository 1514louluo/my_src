#include "memcache_cli_pool.h"
#include "SDConfigurationSingleton.h"

IMPL_LOGGER(memcache_cli_pool, logger);

memcache_cli_pool::memcache_cli_pool(void)
{
}

memcache_cli_pool::~memcache_cli_pool(void)
{
}

void memcache_cli_pool::init_memcache_cli_pool(void )
{
	memcache_client::INIT_MEMCACHED_CLIENT_MODULE();
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();

	int count = config.getInt("memcache_pool_client_count", 10);

	m_memcache_pool_fetch_timeout_threashold = config.getInt("memcache_pool_fetch_timeout_threashold", 500);
	
	for (int i=0; i<count; i++)
	{
		memcache_client *pclient = new memcache_client;
		m_client_queue.add_queue(pclient);
	}

	int lowlevel=config.getInt("memcache_pool_low_water", 50);

	m_pool_low_water = (int)(((float)count)*( (double)lowlevel / 100.0));


}

int memcache_cli_pool::fetch_client( memcache_client **ppclient )
{
	int result;
	int retry_count = 0;
	uint32_t time_before = SDUtility::get_cur_millseconds();
	uint32_t time_after = SDUtility::get_cur_millseconds();
	while (true)
	{
		retry_count++;
		if (retry_count > 5)
		{
			MY_LOGERROR("fatal fetch memcache client from pool timeout");
			PERFORM_LOG("memcache client pool fatal,failed to fetch 5 times");
			*ppclient = NULL;
			return -1;
		}
		memcache_client *pclient = NULL;
		result = m_client_queue.dequeue_expand(pclient);
		if (result != 1)
		{
			continue;
		}

		if (time_after-time_before> m_memcache_pool_fetch_timeout_threashold)
		{
			PERFORM_LOG("memcache client pool fetch timeout, mullsec="<<time_after-time_before);
		}
		//MY_LOGDEBUG("memcache client pool fetch mullsec="<<time_after-time_before);

		int avaliable = m_client_queue.getsize();
		if (avaliable < m_pool_low_water)
		{
			PERFORM_LOG("memcache obj lower  than threashold, avaliable="<<avaliable);

		}
		*ppclient = pclient;
		return 0;
	}

}

int memcache_cli_pool::restore_client( memcache_client *pclient )
{

	m_client_queue.add_queue(pclient);
	return 0;
}
