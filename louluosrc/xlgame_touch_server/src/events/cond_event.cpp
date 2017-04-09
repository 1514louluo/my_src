#include "cond_event.h"
IMPL_LOGGER(cond_event, logger);
cond_event::cond_event(){}
cond_event::~cond_event(){}
void cond_event::init_cond_event(void)
{
	m_filter_ctx = new filter_ctx(COND_PERIOD);
	m_filter_ctx->init_filter_ctx();
}
void cond_event::remove_cond_event(void)
{
	m_filter_ctx->remove_filter_ctx();
	delete m_filter_ctx;
}
char cond_event::get_period(void)
{
	return m_period;
}
int cond_event::do_event(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param) 
{
	return m_filter_ctx->do_filt(request, response_map_param);
}
