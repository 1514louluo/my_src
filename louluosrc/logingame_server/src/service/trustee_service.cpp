

#include "trustee_service.h"
#include "SDConfigurationSingleton.h"
#include "service_stat.h"
#include "commondef.h"
IMPL_LOGGER(trustee_service, logger);







trustee_service::trustee_service(CMysql *pmysql)
{
	m_pmysql = pmysql;
}

trustee_service::~trustee_service(void)
{
}

int trustee_service::get_trusteeinfo_by_id( uint64_t sequenceid , STU_TRUSTEE_INFO *ptrustee_info)
{
	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	if (m_pmysql == NULL)
	{
		MY_LOGERROR("trustee_service m_pmysql == NULL  !!!!!!!!!!!!!!");
		return -1;
	}

	char charmsql[1024] = {0};
	sprintf(charmsql, "select seqid, mastername, mastergid, masteralias, slavername, slavergid,  "
		" slaveralias,serverid, gameid, idcardno,smallsource from gamebox.trustee where seqid=%ld",sequenceid);

	//MY_LOGDEBUG("trustee_service::get_trusteeinfo_by_id  "<<charmsql);
	try{
		m_pmysql->Query(charmsql);
		try
		{
			m_pmysql->StoreResult();
		}
		catch(CMysqlException& e)
		{
			MY_LOGERROR("exec sql failed, sql="<<charmsql<<" error="<<e.GetErrMsg());
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
		std::string source;
		ptrustee_info->seqid	   = sequenceid;
		ptrustee_info->mastername  = m_pmysql->GetFieldBinary("mastername", data_len);
		ptrustee_info->mastergid   = m_pmysql->GetFieldBinary("mastergid", data_len);
		ptrustee_info->masteralias = m_pmysql->GetFieldBinary("masteralias", data_len);
		ptrustee_info->slavername  = m_pmysql->GetFieldBinary("slavername", data_len);
		ptrustee_info->slavergid   = m_pmysql->GetFieldBinary("slavergid", data_len);
		ptrustee_info->slaveralias = m_pmysql->GetFieldBinary("slaveralias", data_len);
		ptrustee_info->serverid    = m_pmysql->GetFieldBinary("serverid", data_len);
		ptrustee_info->gameid      = m_pmysql->GetFieldBinary("gameid", data_len);
		char *pidcardno			   = m_pmysql->GetFieldBinary("idcardno", data_len);
		if (pidcardno != NULL)
		{
			ptrustee_info->idcardno = pidcardno;
		}
		
		//ptrustee_info->idcardno    = m_pmysql->GetFieldBinary("idcardno", data_len);
		char *psource               = m_pmysql->GetFieldBinary("smallsource", data_len);
		if (psource != NULL)
		{
			source					   = psource;
			ptrustee_info->smallsource = SDUtility::atouint32(source.c_str(), source.length());
		}
		else
		{
			ptrustee_info->smallsource = 0;
		}
		
		//source					   = m_pmysql->GetFieldBinary("smallsource", data_len);
		
		return 1;
	}

	return 0;

}

int trustee_service::remove_record_by_id( uint64_t sequenceid )
{
	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	if (m_pmysql == NULL)
	{
		MY_LOGERROR("trustee_service m_pmysql == NULL  !!!!!!!!!!!!!!");
		return -1;
	}

	char charmsql[1024] = {0};
	sprintf(charmsql, "delete from gamebox.trustee where seqid=%ld", sequenceid);

	//MY_LOGDEBUG("trustee_service::remove_record_by_id  "<<charmsql);
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

int trustee_service::update_trustee_info( uint64_t sequenceid, std::string &str_masteralias, 
								std::string &str_slaveralias, std::string &str_gameid, std::string &str_serverid )
{

	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	if (m_pmysql == NULL)
	{
		MY_LOGERROR("trustee_service m_pmysql == NULL  !!!!!!!!!!!!!!");
		return -1;
	}

	char charmsql[4096] = {0};
	sprintf(charmsql, "update  gamebox.trustee set masteralias='%s', slaveralias='%s', gameid='%s',  "
		" serverid='%s'  where seqid=%ld", str_masteralias.c_str(), str_slaveralias.c_str(), 
		 str_gameid.c_str(), str_serverid.c_str(), sequenceid);

	//MY_LOGDEBUG("trustee_service::update_trustee_info  "<<charmsql);
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

int trustee_service::query_slaves( std::string str_master_gameinternalno, std::string str_gameid, std::string str_serverid, std::vector<STU_TRUSTEE_INFO> &vec)
{
	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();
	//MY_LOGINFO("time before="<<timebefore);

	if (m_pmysql == NULL)
	{
		return -1;
	}
	char charmysql[4096]  ={0};
	if (str_gameid.empty())
	{
		sprintf(charmysql, "select seqid,mastergid,mastername,slavergid,slavername,slaveralias,gameid,serverid,masteralias,idcardno,smallsource from gamebox.trustee  "  
			 "  where mastergid='%s'", str_master_gameinternalno.c_str());

	}
	else if (str_serverid == "_")
	{
		sprintf(charmysql, "select seqid,mastergid,mastername,slavergid,slavername,slaveralias,gameid,serverid,masteralias,idcardno,smallsource from gamebox.trustee  " 
			  "  where mastergid='%s' and gameid='%s'  ", str_master_gameinternalno.c_str(), str_gameid.c_str());
	}
	else
	{
		sprintf(charmysql, "select seqid,mastergid,mastername,slavergid,slavername,slaveralias,gameid,serverid,masteralias,idcardno,smallsource from gamebox.trustee  " 
			  "  where mastergid='%s' and gameid='%s'  and  serverid='%s'  ", str_master_gameinternalno.c_str(), str_gameid.c_str(),
				str_serverid.c_str());
	}


	//MY_LOGDEBUG("trustee_service::query_slaves  "<<charmysql);

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
	int record_count = 0;
	while(m_pmysql->FetchRow())
	{
		record_count++;
		std::string source;
		STU_TRUSTEE_INFO stu_trustee_info;
		STU_TRUSTEE_INFO *ptrustee_info = &stu_trustee_info;
		std::string str_sequenceid;
		str_sequenceid             = m_pmysql->GetFieldBinary("seqid", data_len);; 
		ptrustee_info->seqid	   = SDUtility::atouint64(str_sequenceid.c_str(), str_sequenceid.size());
		ptrustee_info->mastername  = m_pmysql->GetFieldBinary("mastername", data_len);
		ptrustee_info->mastergid   = m_pmysql->GetFieldBinary("mastergid", data_len);
		ptrustee_info->masteralias = m_pmysql->GetFieldBinary("masteralias", data_len);
		ptrustee_info->slavername  = m_pmysql->GetFieldBinary("slavername", data_len);
		ptrustee_info->slavergid   = m_pmysql->GetFieldBinary("slavergid", data_len);
		ptrustee_info->slaveralias = m_pmysql->GetFieldBinary("slaveralias", data_len);
		ptrustee_info->serverid    = m_pmysql->GetFieldBinary("serverid", data_len);
		ptrustee_info->gameid      = m_pmysql->GetFieldBinary("gameid", data_len);
		char *pdicardno			   = m_pmysql->GetFieldBinary("idcardno", data_len); 
		if (pdicardno != NULL)
		{
			ptrustee_info->idcardno    = m_pmysql->GetFieldBinary("idcardno", data_len);
		}
		char *psource               = m_pmysql->GetFieldBinary("smallsource", data_len);
		if (psource != NULL)
		{
			source					   = m_pmysql->GetFieldBinary("smallsource", data_len);
			ptrustee_info->smallsource = SDUtility::atouint32(source.c_str(), source.length());
		}
		else
		{
			ptrustee_info->smallsource = 0;
		}

		ptrustee_info->status      = 1;  // 0 为未通过申请状态， 1为处于托管状态

		vec.push_back(stu_trustee_info);

	}

	m_pmysql->FreeResult();

	return record_count;


}



int trustee_service::add_trustee( STU_TRUSTEE_INFO *pstutrustee )
{

	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	if (m_pmysql == NULL)
	{
		return -1;
	}
	char charmysql[4096]  ={0};

	sprintf(charmysql, "insert ignore into gamebox.trustee " 
		  " (mastergid,mastername,slavergid,slavername,masteralias,slaveralias,gameid,serverid,idcardno,smallsource) values "
		"   ('%s',       '%s',        '%s',       '%s',        '%s',         '%s',         '%s',    '%s',      '%s',      %d )",
		    pstutrustee->mastergid.c_str(),
			pstutrustee->mastername.c_str(),
			pstutrustee->slavergid.c_str(),
			pstutrustee->slavername.c_str(),
			pstutrustee->masteralias.c_str(),
			pstutrustee->slaveralias.c_str(),
			pstutrustee->gameid.c_str(),
			pstutrustee->serverid.c_str(),
			pstutrustee->idcardno.c_str(),
			pstutrustee->smallsource);

	MY_LOGDEBUG("trustee_service::add_trustee  "<<charmysql);

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



int trustee_service::query_masters( std::string str_slave_gameinternalno ,std::string gameid,
								   std::string serverid, std::vector<STU_TRUSTEE_INFO> &vec)
{

	if (m_pmysql == NULL)
	{
		return -1;
	}
	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	char charmysql[4096]={0};
	if (gameid.empty())
	{
		sprintf(charmysql, "select seqid,mastergid,mastername,slavergid,slavername,slaveralias,gameid,serverid,masteralias,idcardno,smallsource from gamebox.trustee "
			  " where slavergid='%s'",str_slave_gameinternalno.c_str());
	}
	else if (serverid == "_")
	{
		sprintf(charmysql, "select seqid,mastergid,mastername,slavergid,slavername,slaveralias,gameid,serverid,masteralias,idcardno,smallsource from gamebox.trustee "  
			  " where slavergid='%s' and gameid='%s'", str_slave_gameinternalno.c_str(), gameid.c_str());
	}
	else
	{
		sprintf(charmysql,"select seqid,mastergid,mastername,slavergid,slavername,slaveralias,gameid,serverid,masteralias,idcardno,smallsource from gamebox.trustee " 
			 " where slavergid='%s' and gameid='%s' and serverid='%s'", str_slave_gameinternalno.c_str(), gameid.c_str(), serverid.c_str());

	}

	//MY_LOGDEBUG("trustee_service::query_masters  "<<charmysql);
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
	int record_count = 0;
	while(m_pmysql->FetchRow())
	{
		record_count++;
		std::string source;
		STU_TRUSTEE_INFO stu_trustee_info;
		STU_TRUSTEE_INFO *ptrustee_info = &stu_trustee_info;
		std::string str_sequenceid;
		str_sequenceid             = m_pmysql->GetFieldBinary("seqid", data_len);; 
		ptrustee_info->seqid	   = SDUtility::atouint64(str_sequenceid.c_str(), str_sequenceid.size());
		ptrustee_info->mastername  = m_pmysql->GetFieldBinary("mastername", data_len);
		ptrustee_info->mastergid   = m_pmysql->GetFieldBinary("mastergid", data_len);
		ptrustee_info->masteralias = m_pmysql->GetFieldBinary("masteralias", data_len);
		ptrustee_info->slavername  = m_pmysql->GetFieldBinary("slavername", data_len);
		ptrustee_info->slavergid   = m_pmysql->GetFieldBinary("slavergid", data_len);
		ptrustee_info->slaveralias = m_pmysql->GetFieldBinary("slaveralias", data_len);
		ptrustee_info->serverid    = m_pmysql->GetFieldBinary("serverid", data_len);
		ptrustee_info->gameid      = m_pmysql->GetFieldBinary("gameid", data_len);
		char *pdata                = m_pmysql->GetFieldBinary("idcardno", data_len);
		if (pdata!= NULL)
		{
			ptrustee_info->idcardno    = pdata;
		}
		pdata					   = m_pmysql->GetFieldBinary("smallsource", data_len);
		if (pdata != NULL)
		{
			source					   = pdata;
			//smallsource 申请方式，托管方式申请1          0 通过添加小号申请 
			ptrustee_info->smallsource = SDUtility::atouint32(source.c_str(), source.length());  
		}
		
		ptrustee_info->status      = 1;  // 0 为未通过申请状态， 1为处于托管状态
		vec.push_back(stu_trustee_info);

	}

	return record_count;



}

int trustee_service::query_slavers_count( std::string gameinternalno, int &slavecount )
{

	slavecount = 0;
	if (m_pmysql == NULL)
	{
		return -1;
	}
	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	char charmysql[1024]={0};
	sprintf(charmysql, "select count(mastergid) as slavecount from gamebox.trustee where mastergid='%s'",
			gameinternalno.c_str());

	//MY_LOGDEBUG("trustee_service::query_slavers_count  "<<charmysql);

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
		pdata = m_pmysql->GetFieldBinary("slavecount", data_len);
		slavecount = SDUtility::atouint32(pdata, data_len);
		MY_LOGDEBUG("slavecount trustee = "<<slavecount);
	}

	return 0;



}

int trustee_service::query_record_by_seqid( STU_TRUSTEE_INFO *pstu ,uint64_t seqid)
{
	if (m_pmysql == NULL)
	{
		return -1;
	}
	if (pstu == NULL)
	{
		return -1;
	}
	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	char charmysql[1024]={0};
	sprintf(charmysql, "select seqid,mastergid,mastername,slavergid,slavername,slaveralias,gameid,serverid,masteralias,idcardno,smallsource from gamebox.trustee "
						" where seqid='%ld'", seqid);
	

	//MY_LOGDEBUG("trustee_service::query_record_by_seqid  "<<charmysql);
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
		//record_count++;
		std::string source;
		//STU_TRUSTEE_INFO stu_trustee_info;
		//STU_TRUSTEE_INFO *ptrustee_info = &stu_trustee_info;
		std::string str_sequenceid;
		str_sequenceid    = m_pmysql->GetFieldBinary("seqid", data_len);; 
		pstu->seqid		  = SDUtility::atouint64(str_sequenceid.c_str(), str_sequenceid.size());
		pstu->mastername  = m_pmysql->GetFieldBinary("mastername", data_len);
		pstu->mastergid   = m_pmysql->GetFieldBinary("mastergid", data_len);
		pstu->masteralias = m_pmysql->GetFieldBinary("masteralias", data_len);
		pstu->slavername  = m_pmysql->GetFieldBinary("slavername", data_len);
		pstu->slavergid   = m_pmysql->GetFieldBinary("slavergid", data_len);
		pstu->slaveralias = m_pmysql->GetFieldBinary("slaveralias", data_len);
		pstu->serverid    = m_pmysql->GetFieldBinary("serverid", data_len);
		pstu->gameid      = m_pmysql->GetFieldBinary("gameid", data_len);
		char *pdata       = m_pmysql->GetFieldBinary("idcardno", data_len);
		if (pdata!= NULL)
		{
			pstu->idcardno    = pdata;
		}
		pdata			  = m_pmysql->GetFieldBinary("smallsource", data_len);
		if (pdata != NULL)
		{
			source					   = pdata;
			//smallsource 申请方式，托管方式申请1          0 通过添加小号申请 
			pstu->smallsource = SDUtility::atouint32(source.c_str(), source.length());  
		}

		pstu->status      = 1;  // 0 为未通过申请状态， 1为处于托管状态
		return 1;

	}

	return 0;
}



int trustee_service::updateSlaverIdcardno( std::string &gameinternalno, std::string &cardno )
{
	if (m_pmysql == NULL)
	{
		return -1;
	}
	service_stat *pstat = service_stat::singleton();
	uint64_t timebefore = SDUtility::get_cur_millseconds();

	char charmysql[1096]={0};
	sprintf(charmysql, "update gamebox.trustee set idcardno='%s' where slavergid='%s'",
		  cardno.c_str(), gameinternalno.c_str());

	MY_LOGDEBUG(" trustee_service::updateSlaverIdcardno  "<<charmysql);
	try{
		m_pmysql->Query(charmysql);
		
		
	}
	catch(CMysqlException& e)
	{
		MY_LOGERROR("exec sql failed, sql="<<charmysql<<" error="<<e.GetErrMsg());
		return -1;
	}


	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(DB_RW_TYPE_WRITE,timeafter- timebefore);

	return 0;

}

