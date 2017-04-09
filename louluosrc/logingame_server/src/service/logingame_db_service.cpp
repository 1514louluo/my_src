#include "logingame_db_service.h"
#include "service_stat.h"


std::string logingame_db_service::m_tablename="gamebox.logingame";
IMPL_LOGGER(logingame_db_service, logger);

logingame_db_service::logingame_db_service(CMysql *pmysql)
{
	m_pmysql = pmysql;
}

logingame_db_service::~logingame_db_service(void)
{
}

int logingame_db_service::add_logingame( STU_LOGINGAME_DB_INFO *pstu_logingame_dbinfo )
{

	if (m_pmysql == NULL)
	{
		return -1;
	}

	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	if (m_pmysql == NULL)
	{
		return -1;
	}

	
	char charmsql[1024] = {0};
	sprintf(charmsql, "insert into  %s  "
		 " (gameinternalno,gameid,serverid,logintime) values('%s','%s','%s','%s') "
		 " on duplicate key update logintime='%s' ",get_tablename(pstu_logingame_dbinfo->gameinternalno).c_str(),
		 pstu_logingame_dbinfo->gameinternalno.c_str(), pstu_logingame_dbinfo->gameid.c_str(),
		 pstu_logingame_dbinfo->serverid.c_str(), (SDUtility::get_current_time()).c_str(), (SDUtility::get_current_time()).c_str());

	//MY_LOGDEBUG("logingame_db_service::add_logingame  "<<charmsql);
	try
	{
		m_pmysql->Query(charmsql);
	}
	catch (CMysqlException& e)
	{
		MY_LOGERROR("exec sql failed, sql="<<charmsql<<" error="<<e.GetErrMsg());
		return -1;
	}

	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(DB_RW_TYPE_WRITE,timeafter- timebefore);

	return 0;



}

std::string logingame_db_service::get_tablename( std::string &gameinternalno )
{

	uint64_t id = SDUtility::atouint64(gameinternalno.c_str(), gameinternalno.size());
	id = id%100;
	return m_tablename + SDUtility::format(id);
}

int logingame_db_service::query_user_games( std::string gameinternalno, std::vector<std::string> &vec_games )
{
	if (m_pmysql == NULL)
	{
		return -1;
	}

	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	char charmysql[1024]={0};
	sprintf(charmysql,"select gameid from %s  where gameinternalno='%s'  order by logintime desc",
		get_tablename(gameinternalno).c_str(), gameinternalno.c_str());
	
	MY_LOGDEBUG("logingame_db_service::query_user_games  "<<charmysql);

	try{
		m_pmysql->Query(charmysql);
		try
		{
			m_pmysql->StoreResult();
		}
		catch(CMysqlException& e)
		{
			MY_LOGERROR("exec sql failed, sql="<<charmysql<<" error="<<e.GetErrMsg());
			return -1;
		}
	}
	catch(CMysqlException& e)
	{
		return -1;
	}

	uint64_t timeafter = SDUtility::get_cur_millseconds();
	//MY_LOGINFO("time after="<<timeafter);
	pstat->add_db_stat(DB_RW_TYPE_READ,timeafter- timebefore);
	int data_len;
	
	while(m_pmysql->FetchRow())
	{
		char *pdata = NULL;
		std::string gameid;
		pdata					   = m_pmysql->GetFieldBinary("gameid", data_len); 
		if (pdata != NULL)
		{
			gameid = pdata;
		}

		vec_games.push_back(gameid);
	}

	return 0;



}

int logingame_db_service::queryLastLoginGame( std::string &gameinternalno, std::vector<STU_LOGINGAME_DB_INFO> &vec )
{
	if (m_pmysql == NULL)
	{
		return -1;
	}

	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	char charmysql[2024]={0};


	sprintf(charmysql,"select gameinternalno,gameid,max(concat(logintime,'#',serverid)) as time_serverid from "
		 " %s  where gameinternalno='%s' group by gameinternalno,gameid order by logintime desc",
		get_tablename(gameinternalno).c_str(), gameinternalno.c_str());

	MY_LOGDEBUG("logingame_db_service::query_user_games  "<<charmysql);

	try{
		m_pmysql->Query(charmysql);
		try
		{
			m_pmysql->StoreResult();
		}
		catch(CMysqlException& e)
		{
			MY_LOGERROR("exec sql failed, sql="<<charmysql<<" error="<<e.GetErrMsg());
			return -1;
		}
	}
	catch(CMysqlException& e)
	{
		return -1;
	}

	uint64_t timeafter = SDUtility::get_cur_millseconds();
	//MY_LOGINFO("time after="<<timeafter);
	pstat->add_db_stat(DB_RW_TYPE_READ,timeafter- timebefore);
	int data_len;
	

	while(m_pmysql->FetchRow())
	{
		//char *pdata = NULL;
		STU_LOGINGAME_DB_INFO stu;
		stu.gameinternalno			  = m_pmysql->GetFieldBinary("gameinternalno", data_len); 
		stu.gameid					  = m_pmysql->GetFieldBinary("gameid", data_len); 
		std::string time_serverid;
		time_serverid			      = m_pmysql->GetFieldBinary("time_serverid", data_len); 
		std::vector<std::string> vec_time_serverid;
		boost::split(vec_time_serverid, time_serverid, boost::is_any_of("#"));
		if (vec_time_serverid.size() != 2)
		{
			continue;
		}
		stu.logintime = vec_time_serverid[0];
		stu.serverid = vec_time_serverid[1];
		vec.push_back(stu);
	}


	return 0;




}

int logingame_db_service::gamelogininfos_to_json( std::vector<STU_LOGINGAME_DB_INFO> &vec, std::string &str_json )
{
	
	int size = 0;
	if ( vec.size() > 0) 
	{
		size = vec.size();
	}
	str_json.append("{");
	str_json.append("\"0\":").append(SDUtility::format(size));
	int i=0;
	if (size > 0) 
	{
		str_json.append(",");

		BOOST_FOREACH(STU_LOGINGAME_DB_INFO info, vec)
		{
			i++;
			str_json = str_json + "\"" + SDUtility::format(i) + "\":";

			std::string strobj;
			// 添加一个json对象
			strobj.append("{");
			strobj.append("\"gameid\":\"").append(info.gameid).append("\",");
			if(info.serverid.empty() || info.serverid == "_" )
			{
				strobj.append("\"serverid\":\"").append("").append("\",");
				strobj.append("\"sid\":\"").append("").append("\",");
			}
			else if(info.serverid.find("_") != std::string::npos)
			{
				std::vector<std::string> vec_serverid_sid;
				boost::split(vec_serverid_sid, info.serverid, boost::is_any_of("_"));
				if(vec_serverid_sid.size() == 2)
				{
					strobj.append("\"serverid\":\"").append(vec_serverid_sid[0]).append("\",");
					strobj.append("\"sid\":\"").append(vec_serverid_sid[1]).append("\",");
				}
			}
			strobj.append("\"lasttime\":\"").append(info.logintime).append("\"");
			strobj.append("}");

			str_json = str_json + strobj + ",";



		}
		/*for (LoginGameVO vo : list) 
		{
			sb.append("\"" +(++i)+"\":");
			sb.append(vo.toJsonString()).append(",");
		}*/
		str_json.erase(str_json.size() - 1);
	}
	str_json.append("}");

	return 0;

}

int logingame_db_service::remove_logingame_info( std::string gameinternalno )
{
	if (m_pmysql == NULL)
	{
		return -1;
	}
	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();
	if (m_pmysql == NULL)
	{
		return -1;
	}
	char charmsql[1024] = {0};
	sprintf(charmsql, "delete from  %s  where gameinternalno='%s'", 
		get_tablename(gameinternalno).c_str(), gameinternalno.c_str());

	MY_LOGDEBUG("logingame_db_service::remove_logingame_info  "<<charmsql);
	try
	{
		m_pmysql->Query(charmsql);
	}
	catch (CMysqlException& e)
	{
		MY_LOGERROR("exec sql failed, sql="<<charmsql<<" error="<<e.GetErrMsg());
		return -1;
	}

	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(DB_RW_TYPE_WRITE,timeafter- timebefore);
	return 0;

}

int logingame_db_service::exist_gameuser( std::string gameinternalno )
{
	if (m_pmysql == NULL)
	{
		return -1;
	}
	char charmysql[512]={0};

	sprintf(charmysql,"select gameinternalno from  %s  where gameinternalno='%s'  limit 1",
		get_tablename(gameinternalno).c_str(), gameinternalno.c_str());

	//MY_LOGDEBUG("logingame_db_service::exist_gameuser  "<<charmysql);

	try{
		m_pmysql->Query(charmysql);
		try
		{
			m_pmysql->StoreResult();
		}
		catch(CMysqlException& e)
		{
			MY_LOGERROR("exec sql failed, sql="<<charmysql<<" error="<<e.GetErrMsg());
			return -1;
		}
	}
	catch(CMysqlException& e)
	{
		return -1;
	}

	


	if(m_pmysql->FetchRow())
	{
		return 1;	
	}


	return 0;



}


