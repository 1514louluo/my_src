#include "mysql_config_loader.h"
#include "SDConfigurationSingleton.h"
mysql_config_loader::mysql_config_loader(void)
{
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	 m_str_mysql_ip = config.getString("mysql_ip","127.0.0.1");
	 m_mysql_port = config.getInt("mysql_port", 8888);
	 m_str_user = config.getString("mysql_user","user");
	 m_str_password = config.getString("mysql_password", "888888");
	 m_str_dbname = config.getString("mysql_dbname", "mysql");
}

mysql_config_loader::~mysql_config_loader(void)
{
}
