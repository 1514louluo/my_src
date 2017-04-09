#include "restrict_cond_filter.h"
IMPL_LOGGER(restrict_cond_filter, logger);
char restrict_cond_filter::get_filter_type(void)
{
	return COND_PERIOD;
}
int restrict_cond_filter::do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param)
{
	LOG4CPLUS_DEBUG(logger, "this is <<restrict_cond_filter>> filting!");

	std::string regular_conf = "";
	int num = 0;
	filter_config &fconfig = Singleton<filter_config>::instance();
	
	for(std::map<std::string, std::string>::const_iterator iter = request->mapParam.begin(); iter != request->mapParam.end(); ++iter)
	{
		std::string index = iter->first;
		std::string value = iter->second;
		std::string configid = fconfig.do_find_in_restrict_cond_config( index, value);
		if(!configid.empty())
		{
			++num;
			if(regular_conf.empty())
				regular_conf.append(configid);
			else
				regular_conf.append("|" + configid );
		}
	}

	if(!regular_conf.empty())
		response_map_param->insert(make_pair("regular_conf", regular_conf));
	if(num) return SUCCESS;
	
	LOG4CPLUS_WARN(logger, "this is <<restrict_cond_filter>> fail!");
	return FILT_FAIL_IN_RESTRICT_COND_PERIORD;
}

