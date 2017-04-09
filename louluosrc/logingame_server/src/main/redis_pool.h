#pragma once

#include "redis_client.h"

#include "mycommon.h"
#include "block_queue.h"

class redis_pool
{
public:
	redis_pool(void);
	~redis_pool(void);

	int init_pool(void);

	int fetch_redis_client(redis_client **ppclient);

	int restore_client(redis_client *pclient);


private:
	DECL_LOGGER(logger);

	block_queue<redis_client *> m_queue_client;
};
