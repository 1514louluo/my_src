#include "redis_pool.h"

#include "SDConfigurationSingleton.h"

IMPL_LOGGER(redis_pool, logger);

redis_pool::redis_pool(void)
{
}

redis_pool::~redis_pool(void)
{
}

int redis_pool::init_pool( void )
{
	redis_client::INIT_REDIS_CLIENT();
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	int client_count = config.getInt("redis_pool_client_count", 10);
	for (int i=0;i<client_count; i++)
	{
		redis_client *pclient = new redis_client;
		m_queue_client.add_queue(pclient);
	}


}

int redis_pool::fetch_redis_client( redis_client **ppclient )
{
	int result;
	redis_client *pclient;
	uint64_t beforetime = SDUtility::get_cur_millseconds();
	for (int i=0; i<3; i++)
	{
		result = m_queue_client.dequeue_expand(pclient);
		if (result != 1)
		{
			continue;
		}
		uint64_t aftertime = SDUtility::get_cur_millseconds();
		if (aftertime-beforetime > 100)
		{
			MY_LOGERROR("fetch redis client from pool delayed,delaytime="<<aftertime-beforetime);
		}
		*ppclient = pclient;
		return 0;
	}

	MY_LOGERROR("fetch redis client from pool failed, try  3 times already");
	return -1;

}

int redis_pool::restore_client( redis_client *pclient )
{
	if (pclient == NULL)
	{
		return -1;
	}
	m_queue_client.add_queue(pclient);
	return 0;

}
