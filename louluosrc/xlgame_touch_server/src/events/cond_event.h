#ifndef _COND_EVENT_H
#define _COND_EVENT_H
#include "base_event.h"
#include "filter_ctx.h"
#include "SDLogger.h"
class cond_event:public base_event
{
public:
	cond_event();
	void init_cond_event(void);
	void remove_cond_event(void);
	char get_period(void);
	~cond_event();
	int do_event(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param); 
private:
	/* data */
	filter_ctx *m_filter_ctx;
	char m_period;

	DECL_LOGGER(logger);
};
#endif
