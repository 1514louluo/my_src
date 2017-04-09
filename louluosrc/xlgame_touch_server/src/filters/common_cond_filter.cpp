#include "common_cond_filter.h"
IMPL_LOGGER(common_cond_filter, logger);

char common_cond_filter::get_filter_type(void)
{
	return COND_PERIOD;
}
int common_cond_filter::do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param)
{
	LOG4CPLUS_DEBUG(logger, "this is <<common_cond_filter>> filting!");	

	std::string unregular_conf = "";
//	int num = 0;
	filter_config &fconfig = Singleton<filter_config>::instance();
	
	for(std::map<std::string, std::string>::const_iterator iter = request->mapParam.begin(); iter != request->mapParam.end(); ++iter)
	{
		std::string index = iter->first;
		std::string value = iter->second;
		std::string configid = fconfig.do_find_in_unrestrict_cond_config( index, value);
		if(!configid.empty())
		{
//			++num;
			if(unregular_conf.empty())
				unregular_conf.append(configid);
			else
				unregular_conf.append("|" + configid );
		}
	}

	if(!unregular_conf.empty())
		response_map_param->insert(make_pair("unregular_conf", unregular_conf));

/*	if(num) return SUCCESS;
	
	LOG4CPLUS_WARN(logger, "this is <<common_cond_filter>> fail!");
	return FILT_FAIL_IN_COMMON_COND_PERIORD;
*/	
	
	return SUCCESS;
}

