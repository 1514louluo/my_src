#include "databasepool.h"
#include "SDConfigurationSingleton.h"


IMPL_LOGGER(databasepool, logger);

databasepool::databasepool(void)
{
}

databasepool::~databasepool(void)
{
}

int databasepool::init_databasepool( void )
{
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	int poolcount = config.getInt("database_poll_count", 8);

	m_str_mysql_ip = config.getString("mysql_ip","127.0.0.1");
	m_mysql_port = config.getInt("mysql_port", 8888);
	m_str_user = config.getString("mysql_user","user");
	m_str_password = config.getString("mysql_password", "888888");
	m_str_dbname = config.getString("mysql_dbname", "mysql");
	m_pool_fetch_timeout_threashold = config.getInt("db_pool_fetch_timeout_threashold", 500);
	
	int lowlevel=config.getInt("db_pool_low_water", 50);
	
	m_pool_low_water = (int)(((float)poolcount)*( (double)lowlevel / 100.0));

	for (int i=0;i<poolcount; i++)
	{
		CMysql *pmysql = new CMysql;
		int result = pmysql->Connect(m_str_mysql_ip.c_str(), m_str_user.c_str(), m_str_password.c_str(),
						m_str_dbname.c_str(), m_mysql_port);
		if (result != 0)
		{
			MY_LOGERROR("failed to connect to mysql server");
		}
		else
		{
			MY_LOGINFO("conn mysql success");
		}

		m_queue_mysql.add_queue(pmysql);


	}
	return 0;

}

int databasepool::restore_mysql( CMysql *pmysql )
{

	m_queue_mysql.add_queue(pmysql);
	return 0;
}



int databasepool::fetch_mysql( CMysql **ppmysql )
{

	uint32_t time_before = SDUtility::get_cur_millseconds();
	uint32_t time_after=0;
	int result;
	int retry_count = 0;
	while (true)
	{
		retry_count++;
		if (retry_count > 5)
		{
			MY_LOGERROR("fatal fetch mysql from pool timeout");
			PERFORM_LOG("mysql db pool fatal,failed to fetch 5 times");
			*ppmysql = NULL;
			return -1;
		}
		CMysql *pmysql = NULL;
		result = m_queue_mysql.dequeue_expand(pmysql);
		if (result != 1)
		{
			continue;
		}

		time_after = SDUtility::get_cur_millseconds();
		if (time_after-time_before>m_pool_fetch_timeout_threashold)
		{
			PERFORM_LOG("mysql db pool fetch timeout warning, delay millsec="<<time_after-time_before);
		}
		//MY_LOGDEBUG("mysql db pool fetch millsec="<<time_after-time_before);

		int avaliable = m_queue_mysql.getsize();
		if (avaliable < m_pool_low_water)
		{
			PERFORM_LOG("mysql obj lower  than threashold, avaliable="<<avaliable);
		}

		*ppmysql = pmysql;
		return 0;
	}
}





