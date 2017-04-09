#ifndef _FILTER_CTX_H
#define _FILTER_CTX_H
#include <string>
#include <vector>
#include "SDLogger.h"
#include "mycommon.h"
#include "base_event.h"

#include "common_cond_filter.h"
#include "common_response_filter.h"
#include "common_task_filter.h"

#include "entry_business_filter.h"
#include "restrict_cond_filter.h"
#include "fixed_content_response_filter.h"

class filter_ctx
{
public:
	filter_ctx(char filter_period);
	int init_filter_ctx(void);
	void remove_filter_ctx(void);
	int do_filt(boost::shared_ptr<HttpRequestPacket> request,  std::map<std::string, std::string> *response_map_param);
	~filter_ctx();

private:
	/* data */
	char m_filters_period;
	std::vector<boost::shared_ptr<base_filter> > m_filters;

	DECL_LOGGER(logger);
};
#endif



