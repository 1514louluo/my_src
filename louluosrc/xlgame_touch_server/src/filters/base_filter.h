#ifndef _BASE_FILTER_H
#define _BASE_FILTER_H
#include "HttpRequestPacket.h"
#include "mycommon.h"
#include "base_event.h"
#include "filter_config.h"
#include "Singleton.h"

class base_filter
{
public:
	base_filter(){};
	virtual ~base_filter(){};
	virtual int do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param)=0;
	virtual char get_filter_type(void) = 0;
};
#endif




