#pragma once


#include "mycommon.h"
#include "commondef.h"

#include "mongo_client.h"

#include "SDLogger.h"
#include "SDUtility.h"
#include "master_slave_conf.h"

class logingame_mongodb_service
{
public:
	logingame_mongodb_service(mongo_client *pclient);
	~logingame_mongodb_service(void);


	int add_logingame(STU_LOGINGAME_DB_INFO *pstu_logingame_dbinfo);

	int query_user_games(std::string gameinternalno, std::vector<std::string> &vec_games);

	int queryLastLoginGame(std::string &gameinternalno, std::vector<STU_LOGINGAME_DB_INFO> &vec);

	int queryLastLoginGameMulti_serverid( std::string &gameinternalno, 
		std::vector<STU_LOGINGAME_DB_INFO> &vec );

	int gamelogininfos_to_json(std::vector<STU_LOGINGAME_DB_INFO> &vec, std::string &str_json);

	int remove_logingame_info(std::string gameinternalno);

	int exist_gameuser(std::string gameinternalno);

	

private:
	DECL_LOGGER(logger);
	static std::string m_db_collectionname;
	mongo_client *m_pmongoclient;
};
