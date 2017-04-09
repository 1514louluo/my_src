#include "task_event.h"
IMPL_LOGGER(task_event, logger);
task_event::task_event(){}
task_event::~task_event(){}
void task_event::init_task_event(void)
{
	m_filter_ctx = new filter_ctx(TASK_PERIOD);
	m_filter_ctx->init_filter_ctx();
}
void task_event::remove_task_event(void)
{
	m_filter_ctx->remove_filter_ctx();
	delete m_filter_ctx;
}
char task_event::get_period(void)
{
	return m_period;
}
int task_event::do_event(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param) 
{
	return m_filter_ctx->do_filt(request, response_map_param);
}
