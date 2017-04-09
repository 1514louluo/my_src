#include "response_event.h"
IMPL_LOGGER(response_event, logger);
response_event::response_event(){}
response_event::~response_event(){}
void response_event::init_response_event(void)
{
	m_filter_ctx = new filter_ctx(RESPONSE_PERIOD);
	m_filter_ctx->init_filter_ctx();
}
void response_event::remove_response_event(void)
{
	m_filter_ctx->remove_filter_ctx();
	delete m_filter_ctx;
}
char response_event::get_period(void)
{
	return m_period;
}
int response_event::do_event(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param) 
{
	return m_filter_ctx->do_filt(request, response_map_param);
}
