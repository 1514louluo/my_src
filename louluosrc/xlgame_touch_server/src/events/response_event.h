#ifndef _RESPONSE_EVENT_H
#define _RESPONSE_EVENT_H
#include "base_event.h"
#include "filter_ctx.h"
#include "SDLogger.h"
class response_event:public base_event
{
public:
	response_event();
	~response_event();
	void init_response_event(void);
	void remove_response_event(void);
	char get_period(void);
	int do_event(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param); 
private:
	/* data */
	filter_ctx *m_filter_ctx;
	char m_period;

	DECL_LOGGER(logger);
};
#endif
