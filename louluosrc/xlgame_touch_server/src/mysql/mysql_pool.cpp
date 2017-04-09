#include "mysql_pool.h"
IMPL_LOGGER(mysql_pool, logger);
mysql_pool::mysql_pool(){}
mysql_pool::~mysql_pool(){}	
int mysql_pool::init_pool(int pool_size, int hold_timeout, const char *host, uint16_t port, 
					const char *user, const char *passwd,  const char *dbname
					,const char *szSocket, unsigned long iClientFlag)
{
	m_hold_timeout = hold_timeout;
	int i = 0;
	for(;i < pool_size; ++i)
	{
		CMysql *pclient = new CMysql;
		int result = pclient->Connect(host, user, passwd, dbname, port, szSocket, iClientFlag);
		if (result)
		{
			MY_LOGERROR("<!!!>failed to connect to mysql server");
			return -1;
		}
		m_spset_mysql_single.add_queue(pclient);
	}	
	MY_LOGINFO("<===>mysql_pool init success!");
	return 0;
}
CMysql *mysql_pool::fetch_single(void)
{
	int result;
	CMysql *pclient;
	uint64_t beforetime = SDUtility::get_cur_millseconds();
	for (int i=0; i<3; i++)
	{
		result = m_spset_mysql_single.dequeue_expand(pclient);
		if (result != 1)
		{
			continue;
		}
		uint64_t aftertime = SDUtility::get_cur_millseconds();
		if (aftertime-beforetime > m_hold_timeout)
		{
			MY_LOGERROR("fetch mysql client from pool delayed,delaytime="<<aftertime-beforetime);
		}
		return pclient;
	}

	MY_LOGERROR("fetch mysql client from pool failed, try  3 times already");
	return NULL;
}
int mysql_pool::reback_single(CMysql *pclient)
{
	if (pclient == NULL)
	{
		return -1;
	}
	m_spset_mysql_single.add_queue(pclient);
	return 0;
}



