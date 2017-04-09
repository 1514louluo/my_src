#pragma once

#include "mycommon.h"
#include "libcurlclient.h"
#include "SDUtility.h"
#include "global_var.h"
#include "SDLogger.h"
#include "SDConfigurationSingleton.h"

class tokenid_service
{
public:
	tokenid_service(void);
	~tokenid_service(void);

	static void INIT_TOKENID_SERVICE(void);

	int fetch_tokenid(std::string uid, std::string gameinternalno, std::string &tokenid);


	int remove_tokenid(std::string tokenid);

	static std::string m_server_domain;
private:
	DECL_LOGGER(logger);
	
	static std::string m_tokenid_server_domain;
	static std::string m_tokenid_server_domain_delete;


};
