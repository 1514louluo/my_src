#ifndef _REDIS_POOL_H
#define _REDIS_POOL_H
#include "logdef.h"
#include "mycommon.h"
#include "block_queue.h"
#include "SDUtility.h"
#include "redis_client.h" 

class redis_pool
{
	public:
		redis_pool();
		~redis_pool();	
		int init_pool(int pool_size, int hold_timeout, std::string &rhost, uint16_t rport, int timeout, int expire);
		redis_client *fetch_single(void);
		int reback_single(redis_client *pclient);
	private:
		block_queue<redis_client *> m_spset_redis_single;
		int m_hold_timeout;

		DECL_LOGGER(logger);
};
#endif //REDIS_POOL_H

