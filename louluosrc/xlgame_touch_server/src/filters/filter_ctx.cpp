#include "filter_ctx.h"
IMPL_LOGGER(filter_ctx, logger);

filter_ctx::filter_ctx(char filter_period):m_filters_period(filter_period){}
filter_ctx::~filter_ctx(){}
int filter_ctx::init_filter_ctx(void)
{
	if(BUSINESS_PERIOD == m_filters_period)
	{
		m_filters.push_back(boost::shared_ptr<entry_business_filter>(new entry_business_filter()));
	}
	else if(COND_PERIOD == m_filters_period)
	{
		m_filters.push_back(boost::shared_ptr<restrict_cond_filter>(new restrict_cond_filter()));
		m_filters.push_back(boost::shared_ptr<common_cond_filter>(new common_cond_filter()));
	}
	else if(RESPONSE_PERIOD == m_filters_period)
	{	
		m_filters.push_back(boost::shared_ptr<fixed_content_response_filter>(new fixed_content_response_filter()));
		m_filters.push_back(boost::shared_ptr<common_response_filter>(new common_response_filter()));
	}
	else if(TASK_PERIOD == m_filters_period)
	{
		m_filters.push_back(boost::shared_ptr<common_task_filter>(new common_task_filter()));
	}
	else
	{
		LOG4CPLUS_ERROR(logger, "Wrong filter type in filter context! ==>"<<m_filters_period);
		return -1;
	}
	return 0;
}
void filter_ctx::remove_filter_ctx(void)
{
	m_filters.clear();
}
int filter_ctx::do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param)
{
	LOG4CPLUS_DEBUG(logger, "this is <<filter_ctx>> filting! type = "<<m_filters_period);
	int ret;
	int size = m_filters.size();
	for(int i = 0; i < size; i++)
	{
		ret = m_filters.at(i)->do_filt(request, response_map_param);
		if(SUCCESS != ret) return ret;
	}
	return ret;
}
