#pragma once

#include "mycommon.h"
#include "SDLogger.h"
#include "cjson_wrapper.h"


class router_msg_service
{
public:
	router_msg_service(void);
	~router_msg_service(void);

	void adapter_to_notify_server_encode_json(std::string msgoriginal,std::string from,
		std::string gameinternalno, std::string &content, std::string &str_json);
};
