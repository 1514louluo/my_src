#pragma once

#include "SDLogger.h"
#include "commondef.h"
#include "global_var.h"
#include "cmysql.h"
#include "mycommon.h"
#include "SDUtility.h"
#include "service_stat.h"

#define   PREVENT_STATUS_ALLOW       0
#define   PREVENT_STATUS_WITH_ID      1
#define   PREVENT_STATUS_WITHOUT_ID     2
//isPrevent: 0�������з����ԣ�1�����з�����  2��δ��д��������Ϣ

class prevent_service
{
public:
	prevent_service(CMysql *pmysql);
	~prevent_service(void);

	int update_prevent(STU_PREVENT_INFO *pprevent_info);

	int remove_prevent(std::string &gameid);

	int query_prevent(std::vector<STU_PREVENT_INFO> &vec, std::string str_gameid);

	static void INIT_PREVENT(void);

private:
	CMysql *m_pmysql;
	static std::string m_tablename;
	static std::map<std::string, boost::shared_ptr<STU_PREVENT_INFO> > m_map_prevent;
	
	static MyMutex m_mutex;
DECL_LOGGER(logger);

};
