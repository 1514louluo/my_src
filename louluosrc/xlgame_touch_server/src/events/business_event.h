#ifndef _BUSINESS_EVENT_H
#define _BUSINESS_EVENT_H
#include "base_event.h"
#include "filter_ctx.h"
#include "SDLogger.h"
class business_event:public base_event
{
public:
	business_event();
	void init_business_event(void);
	void remove_business_event(void);
	char get_period(void);
	~business_event();
	int do_event(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param) ;
private:
	/* data */
	filter_ctx *m_filter_ctx;
	char m_period;

	DECL_LOGGER(logger);
};
#endif
