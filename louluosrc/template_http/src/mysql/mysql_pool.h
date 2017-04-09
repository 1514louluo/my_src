#ifndef _MYSQL_POOL_H
#define _MYSQL_POOL_H
#include "logdef.h"
#include "mycommon.h"
#include "block_queue.h"
#include "SDUtility.h"
#include "cmysql.h" 

class mysql_pool
{
	public:
		mysql_pool();
		~mysql_pool();	
		int init_pool(int pool_size, int hold_timeout, const char *host, uint16_t port, 
					const char *user, const char *passwd,  const char *dbname
					,const char *szSocket=NULL, unsigned long iClientFlag=0);
		CMysql *fetch_single(void);
		int reback_single(CMysql *pclient);
	private:
		block_queue<CMysql *> m_spset_mysql_single;
		int m_hold_timeout;

		DECL_LOGGER(logger);
};
#endif //_MYSQL_POOL_H



