#include "mysql_guard.h"

#include "databasepool.h"




IMPL_LOGGER(mysql_guard, logger);


mysql_guard::mysql_guard(void *pdbpool)
{
	m_pmysql = NULL;
	m_pparent = pdbpool;
}

mysql_guard::~mysql_guard(void)
{
	if (m_pparent != NULL && m_pmysql != NULL)
	{

		databasepool *pdatabasepool = (databasepool *)m_pparent;
		pdatabasepool->restore_mysql(m_pmysql);
	}

}

//void mysql_guard::set_mysql( CMysql *pmysql )
//{
//	m_pmysql = pmysql;
//
//}

CMysql * mysql_guard::get_mysql( void )
{
	return m_pmysql;
}
 
int mysql_guard::fetch_mysql_from_pool( void )
{

	if (m_pparent == NULL)
	{
		MY_LOGERROR("mysql_guard::fetch_mysql_from_pool  m_pparent == NULL  ");
		return -1;
	} 
	if (m_pmysql != NULL)
	{
		// 当前guard对象已经从pool里面获取了一个数据库连接对象，直接返回即可
		return 0;
	}
	

	databasepool *pdatabasepool = (databasepool *)m_pparent;
	CMysql *pmysql = NULL;
	int fetch_result = pdatabasepool->fetch_mysql(&pmysql);
	if (fetch_result != 0)
	{
		MY_LOGERROR("fetch db fetch_mysql() failed");
		return -1;
	}
	m_pmysql = pmysql;
	return 0;

	
}


