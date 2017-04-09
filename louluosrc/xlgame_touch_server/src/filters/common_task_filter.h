#ifndef _COMMON_TASK_FILTER_H
#define _COMMON_TASK_FILTER_H
#include "base_filter.h"
#include "SDLogger.h"
class common_task_filter:public base_filter
{
	public:
		char get_filter_type(void);
		int do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param);
	private:
		/*data*/


		DECL_LOGGER(logger);
};
#endif


