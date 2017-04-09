#pragma once

#include "mycommon.h"
#include "SDUtility.h"
#include "SDLogger.h"
#include "SDConfigurationSingleton.h"
#include "commondef.h"
#include "cmysql.h"

class logingame_db_service
{
public:
	logingame_db_service(CMysql *pmysql);
	~logingame_db_service(void);

	int add_logingame(STU_LOGINGAME_DB_INFO *pstu_logingame_dbinfo);

	int query_user_games(std::string gameinternalno, std::vector<std::string> &vec_games);
	
	int queryLastLoginGame(std::string &gameinternalno, std::vector<STU_LOGINGAME_DB_INFO> &vec);

	int gamelogininfos_to_json(std::vector<STU_LOGINGAME_DB_INFO> &vec, std::string &str_json);
	
	int remove_logingame_info(std::string gameinternalno);

	int exist_gameuser(std::string gameinternalno);

private:
	DECL_LOGGER(logger);
	CMysql *m_pmysql;
	static std::string m_tablename;
	std::string get_tablename(std::string &gameinternalno);
};
