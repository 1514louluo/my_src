#pragma once

//#define  _EVENT_DEFINED_TQENTRY  1



// 用于监控当前服务进程的http服务器

//#include "http_monitor_core.h"

#include "logdef.h"
#include "mycommon.h"

#include "SDUtility.h"


#include "event2/event.h"
#include "event2/http.h"

#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>
#define VERSION "1.0"

//TAILQ_HEAD (evkeyvalq, evkeyval);

//#include "http_monitor_core.h"

class http_server
{
public:
	http_server(void);
	~http_server(void);

	void init(void);

	//void set_monitor(http_monitor_core *pparam);
	



private:

	//http_monitor_core *m_p_monitor_core;

	static void api_proxy_handler(struct evhttp_request *req, void *arg);

	void *libevent_http_reactor_doit(void);

	static void *libevent_http_reactor(void *param);

	static std::string m_str_startuptime;
	static std::string m_str_complile_time;

	DECL_LOGGER(logger);
};




