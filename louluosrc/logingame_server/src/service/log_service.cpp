#include "log_service.h"


std::string log_service::m_servername="gameboxgate";
std::string log_service::m_serverip;
std::string log_service::m_loginbox_bizname="loginbox";
std::string log_service::m_logingame_bizname="logingame";
std::string log_service::m_newlogingame_bizname="newlogingame";
std::string log_service::m_slowlog_bizname = "slowlog";

IMPL_LOGGER(log_service, logger);
log_service::log_service(void)
{
}

log_service::~log_service(void)
{
}

void log_service::INIT_LOG_SERVICE( void )
{
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	m_serverip = SDUtility::ip_search();
	zmq_client_pub_Singleton *pclient = zmq_client_pub_Singleton::get_instance();
	pclient->init();


}
int log_service::produce_loginbox_log( HttpRequestPacket *prequest, std::map<std::string, std::string> &parambody )
{
	std::string useLoginState = get_value("loginstate", prequest->mapParam);
	if (useLoginState.empty())
	{  // 0 或 空，为用户名登陆
		useLoginState = "0";
	}

	Router_coder coder;
	coder.setBaseinfo(m_servername, m_serverip, m_loginbox_bizname);
	coder.setData("uid",prequest->str_log_xlinternalno);
	coder.setData("gameinternalno", prequest->str_log_gameinternalno);
	coder.setData("sessionid", prequest->str_log_sessionid);
	coder.setData("username",get_value("username", prequest->mapParam));
	coder.setData("loginstate",useLoginState);
	coder.setData("pid",get_value("pid", prequest->mapParam));
	coder.setData("accountsource",get_value("accountsource", prequest->mapParam));
	coder.setData("startsource",get_value("startsource", prequest->mapParam));
	coder.setData("comefrom",get_value("comefrom", prequest->mapParam));
	coder.setData("result", get_value("result", parambody));
	coder.setData("clientip",get_value("clientip", parambody));
	//coder.setData("comefrom", get_value("comefrom",prequest->mapParam));
	coder.setData("comefrom2", get_value("comefrom2",prequest->mapParam));
	coder.setData("comefrom3", get_value("comefrom3",prequest->mapParam));
	coder.setData("version", get_value("version",prequest->mapParam));

	
	std::string senddata;
	coder.getSendData(senddata);
	zmq_client_pub_Singleton *pclient = zmq_client_pub_Singleton::get_instance();
	pclient->commit(senddata);
	return 0;


}
int log_service::produce_slowlog_log( HttpRequestPacket *prequest, std::map<std::string, std::string> &parambody , time_t nowtime)
{	
	Router_coder coder;
	coder.setBaseinfo(m_servername, m_serverip, m_slowlog_bizname);
	coder.setData("result", get_value("result", parambody));
	coder.setData("starttime", time_t(prequest->m_start_processtime));

	coder.setData("nowtime", nowtime);
	coder.setData("processtime", (time_t)(nowtime - prequest->m_start_processtime));
	coder.setData("request_path", prequest->m_str_cmdpath);
	coder.setData("request_url", prequest->m_str_GetRequest);

	
	std::string senddata;
	coder.getSendData(senddata);
	zmq_client_pub_Singleton *pclient = zmq_client_pub_Singleton::get_instance();
	pclient->commit(senddata);
	return 0;


}

int log_service::produce_logingame_log( HttpRequestPacket *prequest, std::map<std::string, std::string> &parambody )
{
	MY_LOGDEBUG("produce_logingame_log");
	Router_coder coder;
	coder.setBaseinfo(m_servername, m_serverip, m_logingame_bizname);
	coder.setData("sessionid", prequest->str_log_sessionid);
	coder.setData("uid", prequest->str_log_xlinternalno);
	coder.setData("gameinternalno", prequest->str_log_gameinternalno);
	coder.setData("gameid", get_value("gameid", prequest->mapParam));
	coder.setData("serverid", get_value("serverid", prequest->mapParam));
	coder.setData("result", get_value("result", parambody));
	coder.setData("loginstatus", get_value("loginStatus", parambody));
	coder.setData("comefrom", get_value("comefrom",prequest->mapParam));
	coder.setData("comefrom2", get_value("comefrom2",prequest->mapParam));
	coder.setData("comefrom3", get_value("comefrom3",prequest->mapParam));
	coder.setData("version", get_value("version",prequest->mapParam));
	
	std::string senddata;
	coder.getSendData(senddata);
	zmq_client_pub_Singleton *pclient = zmq_client_pub_Singleton::get_instance();
	pclient->commit(senddata);
	return 0;
	
}

int log_service::produce_newlogingame_log( HttpRequestPacket *prequest, std::map<std::string, std::string> &parambody )
{
	MY_LOGDEBUG("produce_newlogingame_log");
	Router_coder coder;
	coder.setBaseinfo(m_servername, m_serverip, m_newlogingame_bizname);
	coder.setData("sessionid", prequest->str_log_sessionid);
	coder.setData("uid", prequest->str_log_xlinternalno);
	coder.setData("gameinternalno", prequest->str_log_gameinternalno);
	coder.setData("gameid", get_value("gameid", prequest->mapParam));
	coder.setData("serverid", get_value("serverid", prequest->mapParam));
	coder.setData("result", get_value("result", parambody));
	coder.setData("loginstatus", get_value("loginStatus", parambody));
	coder.setData("comefrom", get_value("comefrom",prequest->mapParam));
	coder.setData("comefrom2", get_value("comefrom2",prequest->mapParam));
	coder.setData("comefrom3", get_value("comefrom3",prequest->mapParam));
	coder.setData("version", get_value("version",prequest->mapParam));
	
	std::string senddata;
	coder.getSendData(senddata);
	zmq_client_pub_Singleton *pclient = zmq_client_pub_Singleton::get_instance();
	pclient->commit(senddata);
	return 0;
	
}

int log_service::produce_slavelogingame_log( HttpRequestPacket *prequest,
												std::map<std::string, std::string> &parambody )
{
	MY_LOGDEBUG("produce_slavelogingame_log");
	Router_coder coder;
	coder.setBaseinfo(m_servername, m_serverip, m_logingame_bizname);
	coder.setData("sessionid", get_value("sessionid",parambody));
	coder.setData("uid", get_value("uid",parambody));
	coder.setData("gameinternalno", get_value("gameinternalno",parambody));
	coder.setData("gameid", get_value("gameid", parambody));
	coder.setData("serverid", get_value("serverid", parambody));
	coder.setData("result", get_value("result", parambody));
	coder.setData("loginstatus", get_value("loginStatus", parambody));
	coder.setData("comefrom", get_value("comefrom",parambody));
	coder.setData("comefrom2", get_value("comefrom2",parambody));
	coder.setData("comefrom3", get_value("comefrom3",parambody));
	coder.setData("version", get_value("version",parambody));

	std::string senddata;
	coder.getSendData(senddata);
	zmq_client_pub_Singleton *pclient = zmq_client_pub_Singleton::get_instance();
	pclient->commit(senddata);
	return 0;


}



