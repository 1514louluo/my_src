#include "loginwebgame_service.h"
#include "SDConfigurationSingleton.h"
#include "cJSON.h"
#include "global_var.h"
#include "service_stat.h"

IMPL_LOGGER(loginwebgame_service, logger);
std::map<std::string, std::string> loginwebgame_service::m_map_newgameid;
std::string loginwebgame_service::m_old_gameserver;
std::string loginwebgame_service::m_new_gameserver;
std::string loginwebgame_service::m_md5key;
std::string loginwebgame_service::m_gamelogin_rtn_default_errmsg;
std::string loginwebgame_service::m_new_gameserver_retry;
int 		  loginwebgame_service::m_timeout;

loginwebgame_service::loginwebgame_service(void)
{
}

loginwebgame_service::~loginwebgame_service(void)
{
}

int loginwebgame_service::loginGame( std::string &username, std::string &password, std::string &userid, 
					   std::string &copartnerid, std::string &gameid,std::string &sid, 
					   std::string &loginurl, std::string &serverid, std::string &clientip, 
					   STU_GAME_RESULT *presult , int clientid, std::string comefrom,
					   std::string comefrom1,std::string comefrom2,std::string comefrom3, std::string tokenid
					   )
{
	std::string gameLoginStatus_rtn;
	int result;
	STU_GAME_RESULT stu_gameresult;
	result = loginGame_internal(username, password,userid, copartnerid, gameid, sid, loginurl,
								serverid, clientip, &stu_gameresult, clientid, comefrom,
								comefrom1, comefrom2, comefrom3, m_new_gameserver, gameLoginStatus_rtn, tokenid);
	*presult = stu_gameresult;
	if(gameLoginStatus_rtn == "88" || -1 == result)
	{
		// 请求备用机房
		MY_LOGINFO("----------------------logingame  switch to retry url-----------------------------");
		STU_GAME_RESULT stu_gameresult_retry;
		result = loginGame_internal(username, password,userid, copartnerid, gameid, sid, loginurl,
			serverid, clientip, &stu_gameresult_retry, clientid, comefrom,
			comefrom1, comefrom2, comefrom3, m_new_gameserver_retry, gameLoginStatus_rtn,tokenid);

		*presult=stu_gameresult_retry;
		if (stu_gameresult_retry.gameLoginStatus=="88")
		{
			return -2;
		}
		if (result == 0)
		{
			return 0;
		}
		return -1;
	}

	if (result == 0)
	{
		//*presult = stu_gameresult;
		return 0;
	}
	
	
	return -1;
	/*MY_LOGINFO("----------------------logingame  switch to retry url-----------------------------");
	STU_GAME_RESULT stu_gameresult_retry;
	result = loginGame_internal(username, password,userid, copartnerid, gameid, sid, loginurl,
		serverid, clientip, &stu_gameresult_retry, clientid, comefrom,
		comefrom1, comefrom2, comefrom3, m_new_gameserver_retry, gameLoginStatus_rtn);

	*presult=stu_gameresult_retry;
	return result;*/





}


int loginwebgame_service::loginGame_internal ( std::string &username, std::string &password, std::string &userid, 
									std::string &copartnerid, std::string &gameid,std::string &sid, 
									std::string &loginurl, std::string &serverid, std::string &clientip, 
									STU_GAME_RESULT *presult , int clientid, std::string comefrom,
									std::string comefrom1,std::string comefrom2,std::string comefrom3,
									std::string logingameurl, std::string &gameLoginStatus,std::string tokenid
									)
{

	service_stat *pstat = service_stat::singleton();
	int result;
	std::string sign;
	sign = get_url_sign(username,password,userid,copartnerid,gameid,sid,loginurl,serverid,clientip,
						clientid,comefrom,comefrom1,comefrom2,comefrom3, tokenid);
	libcurlclient urlclient;

	/*std::map<std::string, std::string>::iterator iter;
	iter = m_map_newgameid.find(gameid);*/
	std::string request_url;
	/*if (iter != m_map_newgameid.end())
	{
		request_url = m_new_gameserver;
	} 
	else
	{
		uint32_t id = SDUtility::atouint32(gameid.c_str(), gameid.size());
		if (id >= 74 )
		{
			request_url = m_new_gameserver;
		} 
		else
		{
			request_url = m_old_gameserver;
		}
	}*/
	
	/*if (gameid == "000053")
	{
		request_url = m_old_gameserver;
	} 
	else
	{
		request_url = m_new_gameserver;
	}*/
	request_url=logingameurl;

	uint64_t beforetime = time(NULL);
	result = urlclient.http_request_timeout_seconds(request_url,sign, m_timeout);
	uint64_t aftertime = time(NULL);
	if (aftertime>beforetime)
	{
		pstat->add_logingame_duration_stat(aftertime-beforetime);
	} 
	else
	{
		pstat->add_logingame_duration_stat(0);
	}

	if (result == -1)
	{
		presult->gameLoginStatus = "6";
		presult->gameLoginURL = "";
		presult->listNum = "0";
		presult->loginStatus = "0";
		presult->rtnMsg = m_gamelogin_rtn_default_errmsg;
		LOGINGAME_LOG("logingame url-->"<<request_url<<" sign-->"<<sign<<" response-->"<<"SERVER 404");
		return -1;
	}
	LOGINGAME_LOG("logingame url-->"<<request_url<<" sign-->"<<sign<<" response-->"<<urlclient.m_str_response_body);

	std::map<std::string, std::string> map_result;
	string2map(urlclient.m_str_response_body, map_result);

	presult->gameLoginStatus = get_value("gameLoginStatus", map_result);
	presult->gameLoginURL = get_value("gameLoginURL", map_result);
	presult->listNum = get_value("listNum", map_result);
	presult->loginStatus = get_value("loginStatus", map_result);
	presult->rtnMsg = get_value("rtnMsg", map_result);
	presult->costTime = get_value("costTime", map_result);
	gameLoginStatus=presult->gameLoginStatus;
	if (presult->gameLoginStatus.empty() && presult->gameLoginURL.empty() && presult->listNum.empty()
		&& presult->loginStatus.empty() && presult->rtnMsg.empty())
	{
		decode_error_json(urlclient.m_str_response_body, presult);
	}

	return 0;



}


//int loginwebgame_service::loginGame( std::string &username, std::string &password, std::string &userid, 
//									std::string &copartnerid, std::string &gameid,std::string &sid, 
//									std::string &loginurl, std::string &serverid, std::string &clientip, 
//									STU_GAME_RESULT *presult , int clientid, std::string comefrom,
//									std::string startsource)
//{
//
//	int result;
//	std::string sign;
//	//sign = get_url_sign(username,password,userid,copartnerid,gameid,sid,loginurl,serverid,clientip,clientid,comefrom,startsource);
//	libcurlclient urlclient;
//
//	/*std::map<std::string, std::string>::iterator iter;
//	iter = m_map_newgameid.find(gameid);*/
//	std::string request_url;
//	/*if (iter != m_map_newgameid.end())
//	{
//		request_url = m_new_gameserver;
//	} 
//	else
//	{
//		uint32_t id = SDUtility::atouint32(gameid.c_str(), gameid.size());
//		if (id >= 74 )
//		{
//			request_url = m_new_gameserver;
//		} 
//		else
//		{
//			request_url = m_old_gameserver;
//		}
//	}*/
//	
//	if (gameid == "000053")
//	{
//		request_url = m_old_gameserver;
//	} 
//	else
//	{
//		request_url = m_new_gameserver;
//	}
//
//	result = urlclient.http_request_timeout_seconds(request_url,sign, 10);
//	if (result == -1)
//	{
//		presult->gameLoginStatus = "6";
//		presult->gameLoginURL = "";
//		presult->listNum = "0";
//		presult->loginStatus = "0";
//		presult->rtnMsg = m_gamelogin_rtn_default_errmsg;
//		LOGINGAME_LOG("logingame url-->"<<request_url<<" sign-->"<<sign<<" response-->"<<"SERVER 404");
//		return -1;
//	}
//	LOGINGAME_LOG("logingame url-->"<<request_url<<" sign-->"<<sign<<" response-->"<<urlclient.m_str_response_body);
//
//	std::map<std::string, std::string> map_result;
//	string2map(urlclient.m_str_response_body, map_result);
//
//	presult->gameLoginStatus = get_value("gameLoginStatus", map_result);
//	presult->gameLoginURL = get_value("gameLoginURL", map_result);
//	presult->listNum = get_value("listNum", map_result);
//	presult->loginStatus = get_value("loginStatus", map_result);
//	presult->rtnMsg = get_value("rtnMsg", map_result);
//
//	if (presult->gameLoginStatus.empty() && presult->gameLoginURL.empty() && presult->listNum.empty()
//		&& presult->loginStatus.empty() && presult->rtnMsg.empty())
//	{
//		decode_error_json(urlclient.m_str_response_body, presult);
//	}
//
//	return 0;
//
//}

int loginwebgame_service::INIT_LOGINWEBGAME( void )
{
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	m_old_gameserver = config.getString("old_gameserver","websvr.youxi.xunlei.com1");
	m_new_gameserver = config.getString("new_gameserver","websvr.niu.xunlei.com1");
	m_new_gameserver_retry = config.getString("new_gameserver_retry","websvr2.niu.xunlei.com");
	m_md5key = config.getString("weblogingame_md5key","weblogingame_md5key");
	m_timeout = config.getInt("curl_timeout", 20);
	std::string newids;
	newids = config.getString("new_gameid","000075");
	std::vector<std::string> vec;
	boost::split(vec,newids, boost::is_any_of(","));
	BOOST_FOREACH(std::string str, vec)
	{
		if (str.empty())
		{
			continue;
		}
		m_map_newgameid.insert(std::make_pair(str,"1"));
	}
	MY_LOGINFO("load "<<m_map_newgameid.size()<<" new game items");

	
	m_gamelogin_rtn_default_errmsg="登陆服务器IO异常";
	SDUtility::GBKToUTF8(m_gamelogin_rtn_default_errmsg);

	return 0;
}
 

//std::string loginwebgame_service::get_url_sign(std::string &username, std::string &password, std::string &userid, 
//											   std::string &copartnerid, std::string &gameid,
//						  std::string &sid, std::string &loginurl, std::string &serverid, std::string &clientip,
//							int clientid,std::string comefrom,std::string startsource)
//{
//	
//	std::string securty;
//
//	std::string str_url_decoded;
//	str_url_decoded = loginurl;
//	SDUtility::GBKToUTF8(str_url_decoded);
//	SDUtility::URLDecode(str_url_decoded);
//	std::string str_clientid = SDUtility::format(clientid);
//	//securty = securty+"2012111001000000"+"8C89A5BA60F7"+"2012111001000000"+"1003"+"000078"+"4000008"+str_url_decoded
//	//	+"1"+"192.168.101.51"+"B6F5F6640A6593FCAC12C62DC0F580F4";
//	//securty = "username=2012111001000000&pwd=8C89A5BA60F7&userid=2012111001000000&copartnerid=1003&gameid=000078&sid=4000008&loginurl=http:%2F%2Fs1.sgyy.niu.xunlei.com%2Flogin.php&serverid=1&sign=e49b06ea26961a4e05d1e770f6964b1d&clientip=192.168.101.51";
//	securty = username + password + userid + copartnerid + gameid + sid + str_url_decoded + serverid + clientip
//		+ m_md5key;
//
//	unsigned char md5result[100] = {0};
//	unsigned char *pdata;
//	pdata = MD5((const unsigned char*)securty.c_str(), (unsigned long)securty.size(), (unsigned char *)md5result);
//
//	
//	char buf[100]={0};
//	for (int i = 0; i < 16; i++)
//	{
//		char tmp[10]={0};
//		sprintf(tmp,"%2.2x",pdata[i]);
//		strcat(buf,tmp);
//	}
//
//	//printf("%s\n",buf);
//	std::string md5sign;
//	md5sign = buf;
//
//	std::string sb;
//	sb = sb + "username="+username+"&pwd="+password+"&userid="+userid+"&copartnerid="+copartnerid
//		+"&gameid="+gameid+"&sid="+sid+"&loginurl="+loginurl+"&serverid="+serverid+"&sign="+md5sign
//		+"&clientip="+clientip+"&clientid="+str_clientid+"&comefrom="+comefrom+"&comefrom2="+startsource;
//	return sb;
//}


std::string loginwebgame_service::get_url_sign(std::string &username, std::string &password, std::string &userid, 
											   std::string &copartnerid, std::string &gameid,
											   std::string &sid, std::string &loginurl, std::string &serverid, std::string &clientip,
											   int clientid,std::string comefrom,std::string comefrom1,
											   std::string comefrom2,std::string comefrom3,std::string tokenid
												)
{

	std::string securty;

	std::string str_url_decoded;
	str_url_decoded = loginurl;
	SDUtility::GBKToUTF8(str_url_decoded);
	SDUtility::URLDecode(str_url_decoded);
	std::string str_clientid = SDUtility::format(clientid);
	//securty = securty+"2012111001000000"+"8C89A5BA60F7"+"2012111001000000"+"1003"+"000078"+"4000008"+str_url_decoded
	//	+"1"+"192.168.101.51"+"B6F5F6640A6593FCAC12C62DC0F580F4";
	//securty = "username=2012111001000000&pwd=8C89A5BA60F7&userid=2012111001000000&copartnerid=1003&gameid=000078&sid=4000008&loginurl=http:%2F%2Fs1.sgyy.niu.xunlei.com%2Flogin.php&serverid=1&sign=e49b06ea26961a4e05d1e770f6964b1d&clientip=192.168.101.51";
	securty = username + password + userid + copartnerid + gameid + sid + str_url_decoded + serverid + clientip
		+ m_md5key;

	unsigned char md5result[100] = {0};
	unsigned char *pdata;
	pdata = MD5((const unsigned char*)securty.c_str(), (unsigned long)securty.size(), (unsigned char *)md5result);


	char buf[100]={0};
	for (int i = 0; i < 16; i++)
	{
		char tmp[10]={0};
		sprintf(tmp,"%2.2x",pdata[i]);
		strcat(buf,tmp);
	}

	//printf("%s\n",buf);
	std::string md5sign;
	md5sign = buf;

	std::string sb;
	sb = sb + "username="+username+"&pwd="+password+"&userid="+userid+"&copartnerid="+copartnerid
		+"&gameid="+gameid+"&sid="+sid+"&loginurl="+loginurl+"&serverid="+serverid+"&sign="+md5sign
		+"&clientip="+clientip+"&clientid="+str_clientid+"&comefrom="+comefrom+"&comefrom1="+comefrom1
		+"&comefrom2="+comefrom2+"&comefrom3="+comefrom3+"&tokenId="+tokenid;
	return sb;
}


int loginwebgame_service::parseLoginGame(int weblogingame_rtn_code) 
{
	int errorCode;
	switch (weblogingame_rtn_code) {
		case 0:
			errorCode = SUCCESS;
			break;
		case 20:
			errorCode = LOGIN_GAME_ILLEGAL_ACCESS;
			break;
		case 21:
			errorCode = LOGIN_GAME_PARAM_ERROR;
			break;
		case 22:
			errorCode = MD5_VERIFY_ERROR;
			break;
		default:
			errorCode = OTHER_SYS_ERROR;
			break;
	}
	return errorCode;
}

int loginwebgame_service::decode_error_json( std::string &errstr, STU_GAME_RESULT *presult )
{

	cJSON *pjson= cJSON_Parse(errstr.c_str());
	if (pjson == NULL)
	{
		return -1;
	}
//{"rtnMsg":"发生内部错误,请稍候再试","gameLoginStatus":-1,"listNum":0,"loginStatus":6} 
	char charvalue[30]={0};
	cJSON *item =  cJSON_GetObjectItem(pjson,"rtnMsg");
	if (item != NULL)
	{
		presult->rtnMsg = item->valuestring;
	}
	item = cJSON_GetObjectItem(pjson,"gameLoginStatus");
	if (item != NULL)
	{
		memset(charvalue, 0, 30);
		sprintf(charvalue, "%d", item->valueint);
		presult->listNum = charvalue;
	} 
	item = cJSON_GetObjectItem(pjson,"listNum");
	if (item != NULL)
	{
		memset(charvalue, 0, 30);
		sprintf(charvalue, "%d", item->valueint);
		presult->listNum = charvalue;
	}
	item = cJSON_GetObjectItem(pjson,"loginStatus");
	if (item != NULL)
	{
		memset(charvalue, 0, 30);
		sprintf(charvalue, "%d", item->valueint);
		presult->loginStatus = charvalue;
	}

	cJSON_Delete(pjson);
	return 0;

	
 
}




