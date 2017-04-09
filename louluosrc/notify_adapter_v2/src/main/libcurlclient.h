#pragma once

#include "curl/curl.h"
#include "SDLogger.h"
#include "mycommon.h"
class libcurlclient
{
public:
	libcurlclient(void);
	~libcurlclient(void);

	static void INIT_LIBURLCLIENT(void);

	int http_request(std::string &str_url, std::string &str_post_param);

	int http_request_timeout_seconds(std::string &str_url, std::string &str_post_param, 
		 int timeout);

	std::string m_str_response_header;
	std::string m_str_response_body;

private:
	DECL_LOGGER(logger);
	static long writer_response(void *data, int size, int nmemb, void *content);
	static long writer_header(void *data, int size, int nmemb, void *content);
};
