#include "libmemcache_proxy.h"

libmemcache_proxy::libmemcache_proxy(void)
{
	memcached_st *memc;
	 memc = memcached_create(NULL);

	  //rc=memcached_server_push(memc,server);
}

libmemcache_proxy::~libmemcache_proxy(void)
{
}
