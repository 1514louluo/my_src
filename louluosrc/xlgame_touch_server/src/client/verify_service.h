#pragma once

// 验证码服务器

#include "sdsocket.h"
#include "SDLogger.h"
#include "mycommon.h"
#include "SDUtility.h"
#include "boost/algorithm/string.hpp"
class verify_service
{
public:
	verify_service(void);
	~verify_service(void);

	static void INIT_VERIFY_SERVICE(void);

	int verify_code(std::string str_code, std::string str_codeid, std::string codetype);


private:
	static std::vector<std::string> m_vec_serverip;
	static int m_port;
	static int m_socket_timeout;

	DECL_LOGGER(logger);
	
};
