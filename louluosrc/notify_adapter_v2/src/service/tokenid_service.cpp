#include "tokenid_service.h"


IMPL_LOGGER(tokenid_service, logger);



std::string tokenid_service::m_server_domain;
std::string tokenid_service::m_tokenid_server_domain;
std::string tokenid_service::m_tokenid_server_domain_delete;

tokenid_service::tokenid_service(void)
{
}

tokenid_service::~tokenid_service(void)
{
}

int tokenid_service::fetch_tokenid( std::string uid, std::string gameinternalno,std::string &tokenid )
{
	libcurlclient client;
	//tokenid=gameinternalno+"@"+m_server_domain+"/resourcerandomidxxxx";
	std::map<std::string, std::string> mapparam;
	mapparam.insert(make_pair("uid", uid));
	mapparam.insert(make_pair("domain", m_server_domain));
	mapparam.insert(make_pair("gameinternalno", gameinternalno));
	std::string params;
	map2string(params, mapparam);
	int result;
	uint32_t timeout=10;
	result = client.http_request_timeout_seconds(m_tokenid_server_domain, params,timeout);
	if (result == -1)
	{
		MY_LOGERROR("failed to get tokenid from tokenid manager,url="<<m_tokenid_server_domain<<"  param="<<params);
		return -1;
	}
	MY_LOGDEBUG("fetch_tokenid: request: "<<params<<"  resultbody: "<<client.m_str_response_body);
	std::map<std::string, std::string> mapresult;
	string2map(client.m_str_response_body, mapresult);
	tokenid = get_vaule("tokenid", mapresult);
	if (tokenid.empty())
	{
		return -1;
	}


	return 0;



}

void tokenid_service::INIT_TOKENID_SERVICE( void )
{
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	m_tokenid_server_domain = config.getString("tokenid_server_domain","127.0.0.1:4567");
	m_tokenid_server_domain_delete=config.getString("tokenid_server_domain_delete", "127.0.0.1:4567");
	m_server_domain = config.getString("server_domain","domain");
	
	//m_tokenid_port = config.getInt("tokenid_server_port",0);

}

int tokenid_service::remove_tokenid( std::string tokenid )
{
	//path=/svr/notify_adapter/presence/post?type=offline&tokenid=?

	
	

	/*libcurlclient client;
	int result;
	std::map<std::string, std::string> mapparam;
	mapparam.insert(make_pair("tokenid",tokenid));
	std::string param;
	map2string(param, mapparam);
	result = client.http_request(m_tokenid_server_domain_delete, param);
	if (result == -1)
	{
		MY_LOGERROR("failed to remove_tokenid, tokenid="<<tokenid);
		return -1;
	}
	MY_LOGDEBUG("remove tokenid, reply="<<client.m_str_response_body);*/
	return 0;


}
