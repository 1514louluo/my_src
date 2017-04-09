#pragma once
#include "mycommon.h"
class mysql_config_loader
{
public:
	mysql_config_loader(void);
	~mysql_config_loader(void);


	std::string m_str_mysql_ip;
	int         m_mysql_port;
	std::string m_str_user ;
	std::string m_str_password;
	std::string m_str_dbname;
};
