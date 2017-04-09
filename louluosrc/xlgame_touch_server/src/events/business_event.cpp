#include "business_event.h"
IMPL_LOGGER(business_event, logger);
business_event::business_event()
{
	m_period = BUSINESS_PERIOD;
}
business_event::~business_event(){}

void business_event::init_business_event(void)
{
	m_filter_ctx = new filter_ctx(m_period);
	m_filter_ctx->init_filter_ctx();
}
void business_event::remove_business_event(void)
{
	m_filter_ctx->remove_filter_ctx();
	delete m_filter_ctx;
}
char business_event::get_period(void)
{
	return m_period;
}
int business_event::do_event(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param) 
{
	LOG4CPLUS_INFO(logger, "<=Begin business filter!!!=>");
	return m_filter_ctx->do_filt(request, response_map_param);
}
