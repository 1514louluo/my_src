#pragma once


#include "zmq_client_pub_client.h"
#include "HttpRequestPacket.h"
#include "mycommon.h"
#include "SDConfigurationSingleton.h"
#include "SDUtility.h"
#include "SDLogger.h"
class log_service
{
public:
	log_service(void);
	~log_service(void);

	int produce_loginbox_log(HttpRequestPacket *prequest, std::map<std::string, std::string> &parambody);
	int produce_logingame_log( HttpRequestPacket *prequest, std::map<std::string, std::string> &parambody );
	int produce_newlogingame_log( HttpRequestPacket *prequest, std::map<std::string, std::string> &parambody );
	int produce_slavelogingame_log( HttpRequestPacket *prequest,
		std::map<std::string, std::string> &parambody);
	int produce_slowlog_log( HttpRequestPacket *prequest, std::map<std::string, std::string> &parambody , time_t nowtime);
	

	static void INIT_LOG_SERVICE(void);

private:
	static std::string m_servername;
	static std::string m_serverip;
	static std::string m_loginbox_bizname;
	static std::string m_logingame_bizname;
	static std::string m_newlogingame_bizname;
	static std::string m_slowlog_bizname;


	DECL_LOGGER(logger);
};
