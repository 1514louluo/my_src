#pragma once

#include "redis_client.h"

class redis_cli_guard
{
public:
	redis_cli_guard(void *pparent);
	~redis_cli_guard(void);

	int fetch_redis_client_from_pool(void);

	redis_client *get_client(void);

private:
	redis_client *m_pclient;
	void *m_pparent;

};
