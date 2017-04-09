#include "redis_pool.h"
IMPL_LOGGER(redis_pool, logger);
redis_pool::redis_pool(){}
redis_pool::~redis_pool(){}

int redis_pool::init_pool(int pool_size,  int hold_timeout, std::string &rhost, uint16_t rport, int timeout, int expire)
{
	m_hold_timeout = hold_timeout;
	int i = 0;
	for(;i < pool_size; i++)
	{
		redis_client *pclient = new redis_client(rhost, rport, timeout, expire);
		int result = pclient->connect();
		if(result)
		{
			MY_LOGERROR("<!!!>failed to connect to redis server");
			return -1;
		}
		m_spset_redis_single.add_queue(pclient);
	}	
	
	MY_LOGINFO("<===>redis_pool init success!");
	return 0;
}

redis_client *redis_pool::fetch_single(void)
{
	int result;
	redis_client *pclient;
	uint64_t beforetime = SDUtility::get_cur_millseconds();
	for (int i=0; i<3; i++)
	{
		result = m_spset_redis_single.dequeue_expand(pclient);
		if (result != 1)
		{
			continue;
		}
		uint64_t aftertime = SDUtility::get_cur_millseconds();
		if (aftertime-beforetime > m_hold_timeout)
		{
			MY_LOGERROR("fetch redis client from pool delayed,delaytime="<<aftertime-beforetime);
		}
		return pclient;
	}

	MY_LOGERROR("fetch redis client from pool failed, try  3 times already");
	return NULL;
}

int redis_pool::reback_single(redis_client *pclient)
{
	if (pclient == NULL)
	{
		return -1;
	}
	m_spset_redis_single.add_queue(pclient);
	return 0;
}


