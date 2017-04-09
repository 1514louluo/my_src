#include "common_task_filter.h"
IMPL_LOGGER(common_task_filter, logger);

char common_task_filter::get_filter_type(void)
{
	return TASK_PERIOD;
}
int common_task_filter::do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param)
{
	response_map_param->erase("task_conf");
	return SUCCESS;
}

