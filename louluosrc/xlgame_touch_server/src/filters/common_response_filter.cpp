#include "common_response_filter.h"
IMPL_LOGGER(common_response_filter, logger);
char common_response_filter::get_filter_type(void)
{
	return RESPONSE_PERIOD;
}
int common_response_filter::do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param)
{
	response_map_param->erase("unreglular_rtn_content_conf");
	return SUCCESS;
}
