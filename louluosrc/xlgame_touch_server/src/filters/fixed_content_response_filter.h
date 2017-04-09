#ifndef _FIXED_CONTENT_RESPONSE_FILTER_H
#define _FIXED_CONTENT_RESPONSE_FILTER_H
#include "base_filter.h"
#include "SDLogger.h"
class fixed_content_response_filter:public base_filter
{
	public:
		char get_filter_type(void);
		int do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param);
	private:
		/*data*/

		DECL_LOGGER(logger);
};
#endif


