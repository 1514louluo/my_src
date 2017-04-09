#pragma once

#include "cmysql.h"
#include "SDLogger.h"
class mysql_guard
{
public:
	mysql_guard(void *pdbpool);
	~mysql_guard(void);

	//void set_mysql(CMysql *pmysql);

	CMysql * get_mysql(void);

	int fetch_mysql_from_pool(void);

private:

	CMysql *m_pmysql;

	void *m_pparent;

	DECL_LOGGER(logger);

	
};
