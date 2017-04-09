#include "loginbox_his_service.h"

std::string loginbox_his_service::m_str_tablename="gamebox.loginbox";

IMPL_LOGGER(loginbox_his_service, logger);

loginbox_his_service::loginbox_his_service(CMysql *pmysql)
{
	m_pmysql = pmysql;
}

loginbox_his_service::~loginbox_his_service(void)
{
}


std::string loginbox_his_service::get_tablename( std::string &str )
{
	int hashcode = SDUtility::hash_java(str);
	int tableindex = hashcode%100;
	std::string str_index = SDUtility::format(tableindex);
	return m_str_tablename+str_index;

}


int loginbox_his_service::query_loginbox_his( std::string &str_gameinternalno, STU_LOGINBOX_HIS *pstuloginbox )
{
	if (m_pmysql == NULL)
	{
		return -1;
	}
	std::string tablename = get_tablename(str_gameinternalno);

	char charmysql[1024]={0};
	sprintf(charmysql, "select seqid,gameinternalno,username,clientip,clientcity,days,UNIX_TIMESTAMP(lasttime) as lasttime,UNIX_TIMESTAMP(logintime) as logintime from %s "
		" where gameinternalno='%s'", get_tablename(str_gameinternalno).c_str(), str_gameinternalno.c_str());
	

	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();


	//MY_LOGDEBUG("loginbox_his_service::query_loginbox_his  "<<charmysql);

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
	pstat->add_db_stat(DB_RW_TYPE_READ,timeafter- timebefore);

	int data_len;
	if(m_pmysql->FetchRow())
	{
		
		char *pdata = NULL;
		pstuloginbox->seqid          = 0;
		pstuloginbox->gameinternalno = m_pmysql->GetFieldBinary("gameinternalno", data_len);
		pstuloginbox->username       = m_pmysql->GetFieldBinary("username", data_len);
		pstuloginbox->clientip       = m_pmysql->GetFieldBinary("clientip", data_len);
		pdata                           = m_pmysql->GetFieldBinary("clientcity", data_len);
		if (pdata != NULL)
		{
			pstuloginbox->clientcity = pdata;
		}
		pdata                           = m_pmysql->GetFieldBinary("days", data_len);
		pstuloginbox->days           = SDUtility::atouint32(pdata, data_len);
		pstuloginbox->lasttime       = m_pmysql->GetFieldBinary("lasttime", data_len);
		pstuloginbox->logintime      = m_pmysql->GetFieldBinary("logintime", data_len);
		
		m_pmysql->FreeResult();

		return 1;

	}

	return 0;

}

int loginbox_his_service::add_record( STU_LOGINBOX_HIS *pstuloginbox )
{
	MACRO_CHECK_MASTER_SLAVE(" loginbox_his_service::add_record");
	std::string tablename;
	tablename = get_tablename(pstuloginbox->gameinternalno);
	char charmysql[4096]={0};
	std::string lasttime;
	std::string logintime;
	lasttime = SDUtility::format_time(SDUtility::atouint64(pstuloginbox->lasttime.c_str(), pstuloginbox->lasttime.size()));
	logintime = SDUtility::format_time(SDUtility::atouint64(pstuloginbox->logintime.c_str(), pstuloginbox->logintime.size()));
	sprintf(charmysql, "insert into  %s  "
		  "(gameinternalno,username,clientip,clientcity,days,lasttime,logintime)  "
		  " values( '%s',         '%s',    '%s',    '%s',     %d,   '%s',    '%s' )  "
		  " on duplicate key update username='%s', clientip='%s', clientcity='%s',days=%d, lasttime='%s',logintime='%s' ",get_tablename(pstuloginbox->gameinternalno).c_str(), 
		  pstuloginbox->gameinternalno.c_str(),
		  pstuloginbox->username.c_str(),
		  pstuloginbox->clientip.c_str(),
		  pstuloginbox->clientcity.c_str(),
		  pstuloginbox->days,
		  lasttime.c_str(),
		  logintime.c_str(),
		  pstuloginbox->username.c_str(),
		  pstuloginbox->clientip.c_str(),
		  pstuloginbox->clientcity.c_str(),
		  pstuloginbox->days,
		  lasttime.c_str(),
		  logintime.c_str());

		
	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	if (m_pmysql == NULL)
	{
		MY_LOGERROR("trustee_service m_pmysql == NULL  !!!!!!!!!!!!!!");
		return -1;
	}

	 

	//MY_LOGDEBUG("loginbox_his_service::add_record  "<<charmysql);
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

	return 0;


}

int loginbox_his_service::exist_user_loginbox( std::string str_gameinternalno )
{

	if (m_pmysql == NULL)
	{
		return -1;
	}
	std::string tablename = get_tablename(str_gameinternalno);

	char charmysql[512]={0};
	sprintf(charmysql, "select gameinternalno from %s "
		" where gameinternalno='%s' limit 1", get_tablename(str_gameinternalno).c_str(), str_gameinternalno.c_str());


	MY_LOGDEBUG("loginbox_his_service::exist_user_loginbox  "<<charmysql);

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


	int data_len;
	if(m_pmysql->FetchRow())
	{
		return 1;

	}
	return 0;

}
