#include "fixed_content_response_filter.h"
IMPL_LOGGER(fixed_content_response_filter, logger);

char fixed_content_response_filter::get_filter_type(void)
{
	return RESPONSE_PERIOD;
}
int fixed_content_response_filter::do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param)
{
	LOG4CPLUS_DEBUG(logger, "this is <<entry_business_filter>> filting!");
	std::string reglular_rtn_content_conf = get_value("reglular_rtn_content_conf", response_map_param);
	response_map_param->erase("reglular_rtn_content_conf");
	std::vector<std::string> rs;  
	std::string token = "|";
	stringsplit(reglular_rtn_content_conf, token, rs);
	filter_config &fconfig = Singleton<filter_config>::instance();
	int size = rs.size();
	for(int i = 0; i < size; i++)
	{
		std::string value;
		std::string index = fconfig.do_find_in_fixed_content_config(rs.at(i), value);
		
		response_map_param->insert(make_pair(index,value));
	}
	
	return SUCCESS;
}


