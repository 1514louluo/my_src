#include "tokenid_service.h"


IMPL_LOGGER(tokenid_service, logger);



std::string tokenid_service::m_server_domain;
std::string tokenid_service::m_tokenid_server_domain;


tokenid_service::tokenid_service(void)
{
}

tokenid_service::~tokenid_service(void)
{
}

int tokenid_service::fetch_tokenid( std::string gameinternalno, std::string &tokenid )
{
	libcurlclient client;
	tokenid=gameinternalno+"@"+m_server_domain+"/resourcerandomidxxxx";
	return 0;



}

void tokenid_service::INIT_TOKENID_SERVICE( void )
{
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	m_tokenid_server_domain = config.getString("tokenid_server_domain","127.0.0.1:4567");
	m_server_domain = config.getString("server_domain","domain");
	//m_tokenid_port = config.getInt("tokenid_server_port",0);

}
