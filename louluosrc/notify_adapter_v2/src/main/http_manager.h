#pragma once


#include "httpserver.h"
#include "HttpRequestPacket.h"
#include "HttpResponseHeaderPacket.h"


#define   PATH_BROADCAST   "/notify/adapter/boadcast/push"  


typedef struct _STU_HTTP_RTN
{
	int clientfd;
	uint64_t unify_tag;
	std::string str_send;
	std::string str_log_username;
	std::string str_log_gameinternalno;
}STU_HTTP_RTN;

class http_manager
{
public:
	http_manager(void);
	~http_manager(void);

	int init_http_manager(void);

	static int http_callback(enum_notify_type_http notify_type,const char *pdata, int length, 
		STU_EPOLL_DATA *pstu_epoll_data, void *context);

	int parse_http_request(const char *pdata, int length , int clientfd, uint64_t client_unify_tag); 
	httpserver m_httpserver;

	void set_parent(void *param);

private:
	DECL_LOGGER(logger);

	uint64_t m_requst_count;

	void *m_pparent;
};
