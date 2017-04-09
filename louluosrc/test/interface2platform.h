#ifndef _INTERFACE2PLATFORM_H
#define _INTERFACE2PLATFORM_H
#include "SDLogger.h"
/*for debug*/
#define IP_DBG 10.1.9.143
#define PORT_DBG 7070
#define ADD_DBG "recharge.do"
#define SELECT_DBG "querybonus.do"
#define CONSUME_DBG "consume.do"

#define IP_DBG 10.1.9.143
#define PORT_DBG 7070
#define ADD_DBG "recharge.do"
#define SELECT_DBG "querybonus.do"
#define CONSUME_DBG "consume.do"
class interface2platform
{
	public:
		static int select_accum();
		static int consume_accum();
		static int add_accum();
	private:
		int http_client_connect;
		int http_client_send;
		int http_client_parse_response;	

		string 
};
#endif 
