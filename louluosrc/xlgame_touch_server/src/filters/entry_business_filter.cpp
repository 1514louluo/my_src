#include "entry_business_filter.h"
IMPL_LOGGER(entry_business_filter, logger);

char entry_business_filter::get_filter_type(void)
{
	return BUSINESS_PERIOD;
}

int entry_business_filter::do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param)
{
	LOG4CPLUS_DEBUG(logger, "this is <<entry_business_filter>> filting!");
	
	std::vector<std::string> vconfigid;
	filter_config &fconfig = Singleton<filter_config>::instance();
	int ret = SUCCESS;
	ret = fconfig.do_filt_time_limit_in_business_config(vconfigid);
	if(ret) 
	{
		LOG4CPLUS_ERROR(logger, "do_filt_time_limit error!!!!!!!!!!!!!");
		return FILT_FAIL_IN_ENTRY_BUSINESS_PERIORD;
	}
	ret = fconfig.do_filt_input_limit(response_map_param, vconfigid);
	if(ret) 
	{
		LOG4CPLUS_ERROR(logger, "do_filt_input_limit error!!!!!!!!!!!!!");
		return FILT_FAIL_IN_ENTRY_BUSINESS_PERIORD;
	}
	ret = fconfig.do_filt_output_limit(response_map_param, vconfigid);

	if(ret) 
	{
		LOG4CPLUS_ERROR(logger, "do_filt_output_limit error!!!!!!!!!!!!!");
		return FILT_FAIL_IN_ENTRY_BUSINESS_PERIORD;
	}
	return SUCCESS;
}


