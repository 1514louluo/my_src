#pragma once

#include "mycommon.h"
#include "cmysql.h"
#include "commondef.h"
#include "SDLogger.h"
#include "SDConfigurationSingleton.h"
#include "SDUtility.h"
#include "service_stat.h"
#include "master_slave_conf.h"
class loginbox_his_service
{
public:
	loginbox_his_service(CMysql *pmysql);
	~loginbox_his_service(void);

	int query_loginbox_his(std::string &str_gameinternalno, STU_LOGINBOX_HIS *pstuloginbox);
	
	int add_record(STU_LOGINBOX_HIS *pstuloginbox);

	int exist_user_loginbox(std::string str_gameinternalno);

private:
	CMysql *m_pmysql;

	static std::string m_str_tablename;

	std::string get_tablename(std::string &str);

	DECL_LOGGER(logger);
};
