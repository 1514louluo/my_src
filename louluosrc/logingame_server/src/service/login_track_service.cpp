#include "login_track_service.h"
#include "loginbox_his_service.h"
//#include "logingame_db_service.h"
#include "logingame_mongodb_service.h"
#include "global_var.h"
#include "mysql_config_loader.h"
#include "cJSON.h"
#include "log_service.h"

#include "SDUtility.h"
IMPL_LOGGER(login_track_service, logger);

login_track_service::login_track_service(void)
{
	m_predclient_logingame= NULL;
	m_predclient_loginbox = NULL;
}

login_track_service::~login_track_service(void)
{
}

void login_track_service::init_login_track_service( void )
{

	mysql_config_loader loader;
	int result;

	result = m_mysql_loginbox.Connect(loader.m_str_mysql_ip.c_str(), loader.m_str_user.c_str(), 
		loader.m_str_password.c_str(), loader.m_str_dbname.c_str(), loader.m_mysql_port);
	if (result == -1)
	{
		MY_LOGERROR("failed to connect to mysql");
	}
	//result = m_mysql_logingame.Connect(loader.m_str_mysql_ip.c_str(), loader.m_str_user.c_str(), 
	//	loader.m_str_password.c_str(), loader.m_str_dbname.c_str());
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	std::string mongos_ip=config.getString("mongodb_server_ip","0.0.0.0");
	int mongoport=config.getInt("mongodb_server_port",0);
	std::string mongos_conn_str = mongos_ip+":"+SDUtility::format(mongoport);
	result = m_mongoclient.connect(mongos_conn_str);
	if (result == -1)
	{
		MY_LOGERROR("failed to connect to mongos "<<mongos_conn_str);
	}


//	m_predclient_loginbox = new redis_client;
	redis_client::INIT_REDIS_CLIENT();
	m_predclient_logingame = new redis_client;

	pthread_t threadid;
/*	result = pthread_create(&threadid, NULL, login_track_service::dealwith_logibox_proc, this);
	if (result == -1)
	{
		MY_LOGERROR("start thread dealwith_logibox_proc failed");
	}
*/
	result = pthread_create(&threadid, NULL, login_track_service::dealwith_logingame_proc, this);
	if (result == -1)
	{
		MY_LOGERROR("start thread dealwith_logingame_proc failed");
	}




}

void *login_track_service::dealwith_logingame_proc(void *param)
{

	if (param == NULL)
	{
		return (void *)0;
	}
	login_track_service *pthis = (login_track_service *)param;
	pthis->dealwith_logingame_doit();
	MY_LOGERROR("login_track_service::dealwith_logingame_doit terminated");
	return (void *)0;


}


void login_track_service::dealwith_logingame_doit(void)
{

	while (true)
	{
		int result;
		boost::shared_ptr<STU_TRACK_LOGINGAME_INFO> pstu;
		result = m_queue_logingame.dequeue_expand(pstu);
		if (result != 1)
		{
			continue;
		}

		track_logingame(pstu.get());
		//logingame_db_service logingamedbserver(&m_mysql_logingame);
		logingame_mongodb_service logingamedbserver(&m_mongoclient);
		result = logingamedbserver.add_logingame(&(pstu->stu_logingame_info));
		if (result == -1)
		{
			MY_LOGERROR("failed to add logingame to db");
		}


	}


}


void login_track_service::track_logingame( STU_TRACK_LOGINGAME_INFO *pstu_logingame )
{
	int result;
	result = get_logingame_cache(pstu_logingame->stu_logingame_info.gameinternalno);
	//MY_LOGDEBUG("get logingame cache "<<result<<" item");
	if (result == 1)
	{
		return;
	} 
	set_logingame_cache(pstu_logingame->stu_logingame_info.gameinternalno);
	// 从数据库中读取
	result = get_logingame_db(pstu_logingame->stu_logingame_info.gameinternalno);
	//MY_LOGDEBUG("get logingamedb "<<result<<" item");
	if (result == 1)
	{
		return;
	} 
	else
	{
		// 写日志
		LOGINTRACK_LOG("newgame:["<<pstu_logingame->params<<"&gameinternalno="<<
						pstu_logingame->stu_logingame_info.gameinternalno<<"]");
		
		log_service logservice;
		std::map<std::string, std::string> rtn_map;
	
		rtn_map.insert(make_pair("result", get_value("result", pstu_logingame->request.mapParam)));
		rtn_map.insert(make_pair("loginStatus",get_value("loginStatus",pstu_logingame->request.mapParam)));
		logservice.produce_newlogingame_log(&(pstu_logingame->request), rtn_map);
	}

}

int login_track_service::get_logingame_cache( std::string gameinternalno )
{

	std::string key;
	key = key + PREFIX_EXIST_GAMEUSER + gameinternalno;
	int result;
	std::string value;
	result = m_predclient_logingame->get_value(key,value);
	if (result == 1)
	{
		return 1;
	} 
	else
	{
		return 0;
	}

}


int login_track_service::set_logingame_cache( std::string gameinternalno )
{
	std::string key;
	key = key + PREFIX_EXIST_GAMEUSER + gameinternalno;
	int result;
	std::string value;
	cJSON *pjsonobj = cJSON_CreateObject();
	cJSON_AddStringToObject(pjsonobj, "gameinternalno", gameinternalno.c_str());
	char *pcharvalue = cJSON_PrintUnformatted (pjsonobj);
	value = pcharvalue;
	free(pcharvalue);
	cJSON_Delete(pjsonobj);
	result = m_predclient_logingame->set_value(key,value);
	result = m_predclient_logingame->set_expire(key);
	return result;


}


int login_track_service::get_logingame_db( std::string gameinternalno )
{

	//logingame_db_service logingamedbserver(&m_mysql_logingame);
	logingame_mongodb_service logingamedbserver(&m_mongoclient);
	int result;
	result = logingamedbserver.exist_gameuser(gameinternalno);
	if (result == 1)
	{
		return 1;
	} 
	else
	{
		return 0;
	}

}




