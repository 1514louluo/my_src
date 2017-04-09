#include "prevent_service.h"

#include "SDConfigurationSingleton.h"


std::string prevent_service::m_tablename="gamebox.prevent";
std::map<std::string, boost::shared_ptr<STU_PREVENT_INFO> > prevent_service::m_map_prevent;
MyMutex prevent_service::m_mutex;
IMPL_LOGGER(prevent_service, logger);

prevent_service::prevent_service(CMysql *pmysql)
{
	m_pmysql = pmysql;
}

prevent_service::~prevent_service(void)
{
}

int prevent_service::update_prevent( STU_PREVENT_INFO *pprevent_info )
{

	if (m_pmysql == NULL)
	{
		MY_LOGERROR("prevent_service::update_prevent m_pmysql == NULL  !!!!!!!!!!!!!!");
		return -1;
	}

	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	
	char charmysql[1024]={0};
	if (pprevent_info->gameid.empty())
	{
		sprintf(charmysql, "update %s set isprevent=%s , isadult=%s", m_tablename.c_str(),
			pprevent_info->is_prevent.c_str(), pprevent_info->is_adult.c_str());
	}
	else
	{
		sprintf(charmysql,"insert into %s (gameid,isprevent,isadult) values('%s', %s, %s)  "
			" on duplicate key update isprevent=%s,isadult=%s ",m_tablename.c_str(),
			pprevent_info->gameid.c_str(), pprevent_info->is_prevent.c_str(),pprevent_info->is_adult.c_str(),
			pprevent_info->is_prevent.c_str(),pprevent_info->is_adult.c_str());
	}

	MY_LOGDEBUG("prevent_service::update_prevent  "<<charmysql);
	try
	{
		m_pmysql->Query(charmysql);
	}
	catch (CMysqlException& e)
	{
		MY_LOGERROR("exec sql failed, sql="<<charmysql<<" error="<<e.GetErrMsg());
		return -1;
	}

	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(DB_RW_TYPE_WRITE,timeafter- timebefore);

	if (pprevent_info->gameid.empty())
	{
		scopedlock mylock(m_mutex.get());
		std::map<std::string, boost::shared_ptr<STU_PREVENT_INFO> >::iterator iter;
		for (iter = m_map_prevent.begin(); iter!= m_map_prevent.end(); iter++)
		{
			boost::shared_ptr<STU_PREVENT_INFO> stu = iter->second;
			stu->is_adult = pprevent_info->is_adult;
			stu->is_prevent = pprevent_info->is_prevent;
			//MY_LOGDEBUG("update_prevent  push_back gameid="<<pprevent_info->gameid<<" isadult="<<pprevent_info->is_adult
			//	<<" isprevent="<<pprevent_info->is_prevent);
		}

	} 
	else
	{
		scopedlock mylock1(m_mutex.get());
		std::map<std::string, boost::shared_ptr<STU_PREVENT_INFO> >::iterator iter;
		iter = m_map_prevent.find(pprevent_info->gameid);
		if (iter != m_map_prevent.end())
		{
			boost::shared_ptr<STU_PREVENT_INFO> stu_info1 = iter->second;
			stu_info1->is_adult = pprevent_info->is_adult;
			stu_info1->is_prevent = pprevent_info->is_prevent;
			//MY_LOGDEBUG("update_prevent  push_back gameid="<<stu_info1->gameid<<" isadult="<<stu_info1->is_adult
			//	<<" isprevent="<<stu_info1->is_prevent);
		}
		else
		{
			boost::shared_ptr<STU_PREVENT_INFO> stu_info(new STU_PREVENT_INFO);
			stu_info->gameid = pprevent_info->gameid;
			stu_info->is_adult = pprevent_info->is_adult;
			stu_info->is_prevent = pprevent_info->is_prevent;
			m_map_prevent.insert(make_pair(pprevent_info->gameid, stu_info));
			//MY_LOGDEBUG("update_prevent  push_back gameid="<<stu_info->gameid<<" isadult="<<stu_info->is_adult
			//	<<" isprevent="<<stu_info->is_prevent);
		}
	}
	return 0;


}

int prevent_service::remove_prevent( std::string &gameid )
{

	if (m_pmysql == NULL)
	{
		MY_LOGERROR("prevent_service::remove_prevent m_pmysql == NULL  !!!!!!!!!!!!!!");
		return -1;
	}

	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();


	char charmysql[1024]={0};
	if (gameid.empty())
	{
		sprintf(charmysql, "delete from %s", m_tablename.c_str());
	}
	else
	{
		sprintf(charmysql,"delete from %s where gameid='%s'", m_tablename.c_str(), gameid.c_str());
	}


	MY_LOGDEBUG("prevent_service::remove_prevent  "<<charmysql);
	try
	{
		m_pmysql->Query(charmysql);
	}
	catch (CMysqlException& e)
	{
		MY_LOGERROR("exec sql failed, sql="<<charmysql<<" error="<<e.GetErrMsg());
		return -1;
	}

	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(DB_RW_TYPE_WRITE,timeafter- timebefore);

	if (gameid.empty())
	{
		scopedlock mylock1(m_mutex.get());
		m_map_prevent.clear();
	}
	else
	{
		scopedlock mylock(m_mutex.get());
		std::map<std::string, boost::shared_ptr<STU_PREVENT_INFO> >::iterator iter;
		iter = m_map_prevent.find(gameid);
		if (iter != m_map_prevent.end())
		{
			m_map_prevent.erase(iter);
		}
	}

	return 0;

}

int prevent_service::query_prevent( std::vector<STU_PREVENT_INFO> &vec, std::string str_gameid )
{

	scopedlock mylock(m_mutex.get());
	std::map<std::string, boost::shared_ptr<STU_PREVENT_INFO> >::iterator iter;
	if (str_gameid.empty())
	{
		for (iter = m_map_prevent.begin(); iter!=m_map_prevent.end(); iter++)
		{
			boost::shared_ptr<STU_PREVENT_INFO> stu = iter->second;
			vec.push_back(*stu.get());
			//MY_LOGDEBUG("query_prevent  push_back gameid="<<stu->gameid<<" isadult="<<stu->is_adult
			//	<<" isprevent="<<stu->is_prevent);
		}
		
	} 
	else
	{
		iter = m_map_prevent.find(str_gameid);
		if (iter != m_map_prevent.end())
		{
			boost::shared_ptr<STU_PREVENT_INFO> stu = iter->second;
			vec.push_back(*stu);
			//MY_LOGDEBUG("query_prevent  push_back gameid="<<stu->gameid<<" isadult="<<stu->is_adult
			//	<<" isprevent="<<stu->is_prevent);
		}
	}

	return 0;

}

void prevent_service::INIT_PREVENT( void )
{
	CMysql mysql;
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	std::string m_str_mysql_ip = config.getString("mysql_ip","127.0.0.1");
	int         m_mysql_port = config.getInt("mysql_port", 8888);
	std::string m_str_user = config.getString("mysql_user","user");
	std::string m_str_password = config.getString("mysql_password", "888888");
	std::string m_str_dbname = config.getString("mysql_dbname", "mysql");

	int result = mysql.Connect(m_str_mysql_ip.c_str(), m_str_user.c_str(), m_str_password.c_str(), 
		m_str_dbname.c_str(),m_mysql_port);
	if (result == -1)
	{
		MY_LOGERROR("failed to connect to mysql prevent_service::INIT_PREVENT");
		return;
	}

	char charmysql[1024]={0};
	 
	sprintf(charmysql, "select seqid, gameid, isprevent,isadult from %s", m_tablename.c_str());

	try{
		mysql.Query(charmysql);
		try
		{
			mysql.StoreResult();
		}
		catch(CMysqlException& e)
		{
			MY_LOGERROR("exec sql failed, sql="<<charmysql<<" error="<<e.GetErrMsg());
			mysql.Close();
			return;
		}
	}
	catch(CMysqlException& e)
	{
		mysql.Close();
		return;
	}


	int data_len;
	int record_count = 0;
	while(mysql.FetchRow())
	{
		record_count++;
		std::string source;
		boost::shared_ptr<STU_PREVENT_INFO> stu_prevent_info(new STU_PREVENT_INFO);
		std::string str_sequenceid;
		str_sequenceid             = mysql.GetFieldBinary("seqid", data_len);; 
		stu_prevent_info->seqid	   = SDUtility::atouint64(str_sequenceid.c_str(), str_sequenceid.size());
		stu_prevent_info->gameid    = mysql.GetFieldBinary("gameid", data_len);
		stu_prevent_info->is_prevent= mysql.GetFieldBinary("isprevent", data_len);
		stu_prevent_info->is_adult  = mysql.GetFieldBinary("isadult", data_len);

		m_map_prevent.insert(make_pair(stu_prevent_info->gameid, stu_prevent_info));

	}

	mysql.FreeResult();
	mysql.Close();

	 
	 
	


}
