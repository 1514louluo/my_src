#pragma once


#include "cmysql.h"

#include "SDLogger.h"
#include "mycommon.h"
#include <list>
#include "mysql_guard.h"
#include "block_queue.h"
#include "SDUtility.h"
#include "global_var.h"
class databasepool
{
public:
	databasepool(void);
	~databasepool(void);

	int init_databasepool(void);

	int restore_mysql(CMysql *pmysql);

	

	int fetch_mysql(CMysql **ppmysql);

private:

	//std::list<CMysql *> m_list_mysql;
	block_queue<CMysql *> m_queue_mysql;
	std::string m_str_mysql_ip;
	int         m_mysql_port; 
	std::string m_str_user;
	std::string m_str_password;
	std::string m_str_dbname;
	uint32_t    m_pool_fetch_timeout_threashold;
	uint32_t    m_pool_low_water;

	DECL_LOGGER(logger);
	
};
