#include "logingame_mongodb_service.h"

#include "service_stat.h"



IMPL_LOGGER(logingame_mongodb_service, logger);

std::string logingame_mongodb_service::m_db_collectionname="test.logingame";

logingame_mongodb_service::logingame_mongodb_service(mongo_client *pclient)
{
	m_pmongoclient = pclient;
}

logingame_mongodb_service::~logingame_mongodb_service(void)
{
}

int logingame_mongodb_service::add_logingame( STU_LOGINGAME_DB_INFO *pstu_logingame_dbinfo )
{
	MACRO_CHECK_MASTER_SLAVE(" logingame_mongodb_service::add_logingame");
	try
	{

	
	service_stat *pstat = service_stat::singleton();
	if (m_pmongoclient == NULL)
	{
		return -1;
	}
	
	int result;
	mongo::BSONObjBuilder condition;
	condition.append("gameinternalno", pstu_logingame_dbinfo->gameinternalno);
	std::vector<mongo::BSONObj> vec_result;
	uint64_t timebefore = SDUtility::get_cur_millseconds();
	result = m_pmongoclient->query(m_db_collectionname,condition,vec_result);
	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(MONGO_RW_TYPE_READ,timeafter- timebefore);
	if (result == -1)
	{
		MY_LOGERROR("failed to get logingame_mongodb_service::add_logingame");
		return -1;
	}

	if (vec_result.size() == 0)  // 如果是第一次登陆，则没有记录
	{
		mongo::BSONObjBuilder builder;
		builder.append("gameinternalno", pstu_logingame_dbinfo->gameinternalno);
		m_pmongoclient->insert(m_db_collectionname,builder);

		mongo::BSONObj gameinfo_obj = mongo::BSONObjBuilder().append("serverid",pstu_logingame_dbinfo->serverid)
			.append("gameid",pstu_logingame_dbinfo->gameid).append("logintime",pstu_logingame_dbinfo->logintime).obj();
		mongo::BSONObj addtoset     = mongo::BSONObjBuilder().append("gameinfos", gameinfo_obj).obj();
		mongo::BSONObj update_value = mongo::BSONObjBuilder().append("$push", addtoset).obj();
		mongo::BSONObjBuilder condition;
		condition.append("gameinternalno", pstu_logingame_dbinfo->gameinternalno);

		timebefore = SDUtility::get_cur_millseconds();
		result = m_pmongoclient->update(m_db_collectionname, condition, update_value);
		timeafter = SDUtility::get_cur_millseconds();
		pstat->add_db_stat(MONGO_RW_TYPE_WRITE,timeafter- timebefore);

		if (result == -1)
		{
			MY_LOGERROR("failed to new insert logingame info");
			return -1;
		}
		return 0;
	}
	else
	{
		mongo::BSONObj result_obj;
		result_obj = vec_result[0];
		mongo::BSONObj gameinfoobj = result_obj.getObjectField("gameinfos");
		std::vector<mongo::BSONElement> vec_element;
		gameinfoobj.elems(vec_element);
		MY_LOGDEBUG("logingameinfo vec_element.size()="<<vec_element.size());
		if (vec_element.size() > 20)
		{
			//printf("larger than 20 array, skip\n");
			//continue;
			// 需要做pop操作
			mongo::BSONObjBuilder builder;
			builder.append("gameinternalno", pstu_logingame_dbinfo->gameinternalno);
			mongo::BSONObj popkey = mongo::BSONObjBuilder().append("gameinfos", -1).obj();
			mongo::BSONObj update_value = mongo::BSONObjBuilder().append("$pop", popkey).obj();
			timebefore = SDUtility::get_cur_millseconds();
			result = m_pmongoclient->update(m_db_collectionname, builder, update_value);
			timeafter = SDUtility::get_cur_millseconds();
			pstat->add_db_stat(MONGO_RW_TYPE_WRITE,timeafter- timebefore);

			if (result == -1)
			{
				MY_LOGERROR("failed to pop gameinfos");
				return -1;
			}
 
		}
		
		mongo::BSONObjBuilder builder;
		builder.append("gameinternalno", pstu_logingame_dbinfo->gameinternalno);
		mongo::BSONObj gameinfo_obj = mongo::BSONObjBuilder().append("serverid",pstu_logingame_dbinfo->serverid)
			.append("gameid",pstu_logingame_dbinfo->gameid).append("logintime",pstu_logingame_dbinfo->logintime).obj();
		mongo::BSONObj addtoset     = mongo::BSONObjBuilder().append("gameinfos", gameinfo_obj).obj();
		mongo::BSONObj update_value = mongo::BSONObjBuilder().append("$push", addtoset).obj();
		timebefore = SDUtility::get_cur_millseconds();
		result = m_pmongoclient->update(m_db_collectionname, builder, update_value);
		timeafter = SDUtility::get_cur_millseconds();
		pstat->add_db_stat(MONGO_RW_TYPE_WRITE,timeafter- timebefore);

		if (result == -1)
		{
			MY_LOGERROR("failed to push gameinfos");
			return -1;
		}
		return 0;
		
	}

	return 0;


	}
	catch( std::exception& e ) 
	{
		//cout << "ERROR: " << e.what() << endl;
		MY_LOGDEBUG("ERROR: "<<e.what());
		return -1;
	}


}

int logingame_mongodb_service::query_user_games( std::string gameinternalno, std::vector<std::string> &vec_games )
{
	service_stat *pstat = service_stat::singleton();

	if (m_pmongoclient == NULL)
	{
		return -1;
	}

	try
	{

	
	int result;
	mongo::BSONObjBuilder condition;
	condition.append("gameinternalno", gameinternalno);
	std::string filed;
	filed = "gameinternalno";
	mongo::BSONObj obj_result;
	uint64_t timebefore = SDUtility::get_cur_millseconds();
	result = m_pmongoclient->findone(m_db_collectionname, filed, gameinternalno, obj_result);
	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(MONGO_RW_TYPE_READ,timeafter- timebefore);
	if (result == -1)
	{
		MY_LOGERROR("faild to find one on mongodb");
		return -1;
	}
	
	mongo::BSONObj gameinfosobj = obj_result.getObjectField("gameinfos");
	std::vector<mongo::BSONElement> vec_element;
	gameinfosobj.elems(vec_element);

	BOOST_FOREACH(mongo::BSONElement jsonelement, vec_element)
	{
		std::string gameid;
		mongo::BSONObj gameinfoobj = jsonelement.Obj();
		gameid = gameinfoobj.getStringField("gameid");
		vec_games.push_back(gameid);
	}
	return 0;

	}
	catch( std::exception& e ) 
	{
		//cout << "ERROR: " << e.what() << endl;
		MY_LOGDEBUG("ERROR: "<<e.what());
		return -1;
	}

}

int logingame_mongodb_service::queryLastLoginGame( std::string &gameinternalno, 
												  std::vector<STU_LOGINGAME_DB_INFO> &vec )
{
	service_stat *pstat = service_stat::singleton();
	if (m_pmongoclient == NULL)
	{
		return -1;
	}

	try
	{

	
	int result;
	mongo::BSONObjBuilder condition;
	condition.append("gameinternalno", gameinternalno);
	std::string filed;
	filed = "gameinternalno";
	mongo::BSONObj obj_result;
	uint64_t timebefore = SDUtility::get_cur_millseconds();
	result = m_pmongoclient->findone(m_db_collectionname, filed, gameinternalno, obj_result);
	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(MONGO_RW_TYPE_READ,timeafter- timebefore);

	if (result == -1)
	{
		MY_LOGERROR("faild to find one on mongodb");
		return -1;
	}

	mongo::BSONObj gameinfosobj = obj_result.getObjectField("gameinfos");
	std::vector<mongo::BSONElement> vec_element;
	gameinfosobj.elems(vec_element);

	std::vector<mongo::BSONElement>::reverse_iterator iter_rev;
	for (iter_rev = vec_element.rbegin(); iter_rev!=vec_element.rend(); iter_rev++)
	{
		STU_LOGINGAME_DB_INFO stu;
		mongo::BSONElement jsonelement = *iter_rev;
		mongo::BSONObj gameinfoobj = jsonelement.Obj();
		stu.gameid = gameinfoobj.getStringField("gameid");
		stu.gameinternalno = gameinternalno;
		stu.logintime = gameinfoobj.getStringField("logintime");
		stu.serverid = gameinfoobj.getStringField("serverid");
		bool isgameidexist=false;
		BOOST_FOREACH(STU_LOGINGAME_DB_INFO stuinfo, vec)
		{
			if (stuinfo.gameid == stu.gameid)
			{
				isgameidexist = true;
				break;
			}
		}
		if (isgameidexist == false)
		{
			vec.push_back(stu);
		}

	}
	/*BOOST_FOREACH(mongo::BSONElement jsonelement, vec_element)
	{
		
		
	}*/
	return 0;


	}
	catch( std::exception& e ) 
	{
		//cout << "ERROR: " << e.what() << endl;
		MY_LOGDEBUG("ERROR: "<<e.what());
		return -1;
	}

}


int logingame_mongodb_service::queryLastLoginGameMulti_serverid( std::string &gameinternalno, 
												  std::vector<STU_LOGINGAME_DB_INFO> &vec )
{
	service_stat *pstat = service_stat::singleton();
	if (m_pmongoclient == NULL)
	{
		return -1;
	}

	try
	{

	
	int result;
	mongo::BSONObjBuilder condition;
	condition.append("gameinternalno", gameinternalno);
	std::string filed;
	filed = "gameinternalno";
	mongo::BSONObj obj_result;
	uint64_t timebefore = SDUtility::get_cur_millseconds();
	result = m_pmongoclient->findone(m_db_collectionname, filed, gameinternalno, obj_result);
	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(MONGO_RW_TYPE_READ,timeafter- timebefore);

	if (result == -1)
	{
		MY_LOGERROR("faild to find one on mongodb");
		return -1;
	}

	mongo::BSONObj gameinfosobj = obj_result.getObjectField("gameinfos");
	std::vector<mongo::BSONElement> vec_element;
	gameinfosobj.elems(vec_element);

	std::vector<mongo::BSONElement>::reverse_iterator iter_rev;
	for (iter_rev = vec_element.rbegin(); iter_rev!=vec_element.rend(); iter_rev++)
	{
		STU_LOGINGAME_DB_INFO stu;
		mongo::BSONElement jsonelement = *iter_rev;
		mongo::BSONObj gameinfoobj = jsonelement.Obj();
		stu.gameid = gameinfoobj.getStringField("gameid");
		stu.gameinternalno = gameinternalno;
		stu.logintime = gameinfoobj.getStringField("logintime");
		stu.serverid = gameinfoobj.getStringField("serverid");
		/*bool isgameidexist=false;
		BOOST_FOREACH(STU_LOGINGAME_DB_INFO stuinfo, vec)
		{
			if (stuinfo.gameid == stu.gameid)
			{
				isgameidexist = true;
				break;
			}
		}
		if (isgameidexist == false)
		{*/
		vec.push_back(stu);
		//}

	}
	/*BOOST_FOREACH(mongo::BSONElement jsonelement, vec_element)
	{
		
		
	}*/
	return 0;


	}
	catch( std::exception& e ) 
	{
		//cout << "ERROR: " << e.what() << endl;
		MY_LOGDEBUG("ERROR: "<<e.what());
		return -1;
	}

}



int logingame_mongodb_service::gamelogininfos_to_json( std::vector<STU_LOGINGAME_DB_INFO> &vec, std::string &str_json )
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


int logingame_mongodb_service::remove_logingame_info( std::string gameinternalno )
{
	MACRO_CHECK_MASTER_SLAVE(" logingame_mongodb_service::remove_logingame_info");
	service_stat *pstat = service_stat::singleton();
	if (m_pmongoclient == NULL)
	{
		return -1;
	}
	
	int result;
	uint64_t timebefore = SDUtility::get_cur_millseconds();
	result = m_pmongoclient->remove(m_db_collectionname, "gameinternalno", gameinternalno);
	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(MONGO_RW_TYPE_WRITE,timeafter- timebefore);

	if (result == -1)
	{
		MY_LOGERROR("failed to remove logingameinfo, gameinternalno="<<gameinternalno);
		return -1;
	}
	return 0;

}

int logingame_mongodb_service::exist_gameuser( std::string gameinternalno )
{

	service_stat *pstat = service_stat::singleton();
	if (m_pmongoclient == NULL)
	{
		return -1;
	}

	int result;
	mongo::BSONObjBuilder condition;
	condition.append("gameinternalno", gameinternalno);
	std::string filed;
	filed = "gameinternalno";
	mongo::BSONObj obj_result;
	uint64_t timebefore = SDUtility::get_cur_millseconds();
	result = m_pmongoclient->findone(m_db_collectionname, filed, gameinternalno, obj_result);
	uint64_t timeafter = SDUtility::get_cur_millseconds();
	pstat->add_db_stat(MONGO_RW_TYPE_READ,timeafter- timebefore);
	if (result == -1)
	{
		MY_LOGERROR("faild to find one on mongodb");
		return -1;
	}
	//MY_LOGDEBUG("findone="<<obj_result.jsonString());
	//std::string gameinternalno_exist = obj_result.getStringField("gameinternalno");
	if (obj_result.isEmpty())
	{
		return 0;
	} 
	else
	{
		return 1;
	}
	/*std::string str_result;
	str_result = gameinternalno_obj.jsonString();
	MY_LOGDEBUG("str_result="<<str_result);
	if (str_result.empty())
	{
		return 0;
	} 
	else
	{
		return 1;
	}*/



}
