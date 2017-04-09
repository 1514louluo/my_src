#ifndef _BASE_EVENT_H
#define _BASE_EVENT_H
#include "HttpRequestPacket.h"
#include "mycommon.h"
#include "commondef.h"

#define BUSINESS_PERIOD                     '0'
#define COND_PERIOD                            '1'
#define RESPONSE_PERIOD                    '2'
#define TASK_PERIOD                            '3'
class base_event
{
public:
	base_event(){};
	virtual int do_event(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param) = 0;
	virtual ~base_event(){};
	/* data */
};
#endif


