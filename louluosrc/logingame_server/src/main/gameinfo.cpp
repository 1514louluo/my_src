#include "gameinfo.h"
#include "cJSON.h"
IMPL_LOGGER(gameinfo, logger);

gameinfo::gameinfo()
{
	pthread_rwlock_init(&m_rwlock, NULL);
	pthread_rwlock_init(&m_spe_rwlock, NULL);
	get_gameinfo_from_db();
	get_gameinfo_from_db_4spe();
}
gameinfo::~gameinfo(){}

gameinfo* gameinfo::instance = NULL;
gameinfo* gameinfo::GetInstance()
{
	if(NULL == instance)
		instance = new gameinfo();
	return instance;
}
int gameinfo::get_gameinfo4special(std::string &gameid, std::string &serverid, std::string &sid, std::string &loginurl)
{
	loginurl = "";
	std::string key_index = gameid + serverid + sid;
	while(pthread_rwlock_tryrdlock(&m_spe_rwlock))
		usleep(3000);

   	for(map<string, string>::const_iterator iter = m_gameinfo_4spe.begin(); iter != m_gameinfo_4spe.end(); ++iter)
   	{
		std::string key = iter->first;

        /*parse*/
        	if(key == key_index) 
		{
			loginurl = iter->second;	
			break;
		}
    	}
	pthread_rwlock_unlock(&m_spe_rwlock);
	if("" == loginurl) return -1;
	return 0;
}
int gameinfo::get_gameinfo(std::string &gameid, std::string &serverid, std::string &sid, std::string &loginurl, std::string &gameinfo)
{
	std::string value = "";
	while(pthread_rwlock_tryrdlock(&m_rwlock))
		usleep(3000);

   	for(map<string, string>::const_iterator iter = m_gameinfo.begin(); iter != m_gameinfo.end(); ++iter)
   	{
		std::string key = iter->first;

        /*parse*/
        	if(key == gameid) 
		{
			value = iter->second;	
			break;
		}
    	}
	pthread_rwlock_unlock(&m_rwlock);
	if("" == value) return -1;
//	MY_LOGINFO(value);
	unsigned int loc = value.find('}', 0);
       if(loc == std::string::npos) loc = value.size() - 1;

	std::string phalf(value, 0, loc+1);
	std::string bhalf(value, loc+1, value.size() - loc - 1);
	   
	cJSON *root =  cJSON_Parse(phalf.c_str());
	cJSON *root_serverid = cJSON_GetObjectItem(root, "serverid");
	cJSON *root_sid = cJSON_GetObjectItem(root, "sid");
	cJSON *root_loginurl = cJSON_GetObjectItem(root, "loginurl");
	
	serverid = root_serverid->valuestring;
	sid          = root_sid->valuestring;
	loginurl   = root_loginurl->valuestring;
	gameinfo = bhalf;
	
	cJSON_Delete(root);
 
	return 0;
	
}
int gameinfo::get_gameinfo_from_db_4spe(void)
{
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	 std::string str_mysql_ip = config.getString("gameinfo_mysql_ip_4spe","127.0.0.1");
	 int str_port = config.getInt("gameinfo_mysql_port_4spe", 8888);
	 std::string str_user = config.getString("gameinfo_mysql_user_4spe","user");
	 std::string str_password = config.getString("gameinfo_mysql_password_4spe", "888888");
	 std::string str_dbname = config.getString("gameinfo_mysql_dbname_4spe", "mysql");
	 std::string str_tname = config.getString("gameinfo_mysql_tname_4spe", "mysql");
	 CMysql mysql;
	 int ret = mysql.Connect(str_mysql_ip.c_str(), str_user.c_str(), str_password.c_str(),
				   str_dbname.c_str(), str_port, NULL, 0);

     if (ret)
     {
     	LOG4CPLUS_ERROR(logger, "connect error!");
       	return -1;
     }

     char charmsql[1024] = {0};
     sprintf(charmsql,"select gameid,serverid,sid,loginurl from "
   			" %s ",
           str_tname.c_str());

     try{
     	mysql.Query(charmsql);
       	try
       	{
       		mysql.StoreResult();
        }
        catch(CMysqlException& e)
        {
        	LOG4CPLUS_ERROR(logger, "Mysql StoreResult exception!");
           	return -1;
        }
     }
    catch(CMysqlException& e)
    {
    	LOG4CPLUS_ERROR(logger, "Mysql select gameinfo exception!");
       	return -1;
    }

    int data_len;

	while(pthread_rwlock_trywrlock(&m_spe_rwlock))
		usleep(3000);

	m_gameinfo_4spe.clear();
    while(mysql.FetchRow())
    {
		std::string _gameid        = mysql.GetFieldBinary("gameid", data_len);
		std::string _serverid      = mysql.GetFieldBinary("serverid", data_len);
		std::string _sid               = mysql.GetFieldBinary("sid", data_len);
		std::string _loginurl       = mysql.GetFieldBinary("loginurl", data_len);
		
		m_gameinfo_4spe.insert(make_pair(_gameid + _serverid + _sid,  _loginurl));
		                                                            
    }
	pthread_rwlock_unlock(&m_spe_rwlock);

    mysql.Close();
    return 0;
}
int gameinfo::get_gameinfo_from_db(void)
{
	 const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	 std::string str_mysql_ip = config.getString("gameinfo_mysql_ip","127.0.0.1");
	 int str_port = config.getInt("gameinfo_mysql_port", 8888);
	 std::string str_user = config.getString("gameinfo_mysql_user","user");
	 std::string str_password = config.getString("gameinfo_mysql_password", "888888");
	 std::string str_dbname = config.getString("gameinfo_mysql_dbname", "mysql");
	 std::string str_tname = config.getString("gameinfo_mysql_tname", "mysql");
	 CMysql mysql;
	 int ret = mysql.Connect(str_mysql_ip.c_str(), str_user.c_str(), str_password.c_str(),
				   str_dbname.c_str(), str_port, NULL, 0);

     if (ret)
     {
     	LOG4CPLUS_ERROR(logger, "connect error!");
       	return -1;
     }

     char charmsql[1024] = {0};
     sprintf(charmsql,"select gameid,serverid,sid,loginurl,params from "
   			" %s ",
           str_tname.c_str());

     try{
     	mysql.Query(charmsql);
       	try
       	{
       		mysql.StoreResult();
        }
        catch(CMysqlException& e)
        {
        	LOG4CPLUS_ERROR(logger, "Mysql StoreResult exception!");
           	return -1;
        }
     }
    catch(CMysqlException& e)
    {
    	LOG4CPLUS_ERROR(logger, "Mysql select gameinfo exception!");
       	return -1;
    }

    int data_len;

	while(pthread_rwlock_trywrlock(&m_rwlock))
		usleep(3000);

	m_gameinfo.clear();
    while(mysql.FetchRow())
    {
		std::string _gameid        = mysql.GetFieldBinary("gameid", data_len);
		std::string _serverid      = mysql.GetFieldBinary("serverid", data_len);
		std::string _sid               = mysql.GetFieldBinary("sid", data_len);
		std::string _loginurl       = mysql.GetFieldBinary("loginurl", data_len);
		std::string _params       = mysql.GetFieldBinary("params", data_len);
		
		m_gameinfo.insert(make_pair(_gameid, "{\"serverid\":\"" + _serverid + "\",\"sid\":\"" + _sid + "\",\"loginurl\":\"" + _loginurl + "\"}"+_params));
		                                                            
    }
	pthread_rwlock_unlock(&m_rwlock);

    mysql.Close();
    return 0;
}
