#ifndef _FILTER_CONFIG_H
#define _FILTER_CONFIG_H
#include "RW_detacher.h"
#include "SDPthreadLock.h"
#include "SDLogger.h"
#include "SDConfigurationSingleton.h"
#include "commondef.h"
#include "mycommon.h"
#include <boost/noncopyable.hpp>

typedef struct fixed_content_response_config
{
	std::string m_uniq_tag;
	std::string m_type;
	std::string m_estimated_value;  
}fixed_content_response_config;

typedef struct restrict_cond_config
{
    		std::string m_uniq_tag;
		std::string m_control_identity;
		std::string m_estimated_value;
}restrict_cond_config;		

typedef struct unrestrict_cond_config
{
		std::string m_uniq_tag;
		std::string m_control_identity;
		std::string m_estimated_value;
}unrestrict_cond_config;

typedef struct business_config
{
	    	std::string m_configid;
		std::string m_valid_start_unixtime;
		std::string m_regular_conf;
		std::string m_unregular_conf;
		std::string m_regular_rtn_content_conf;
		std::string m_unregular_rtn_content_conf;
		std::string m_task_conf;
		std::string m_volumn;	

}business_config;

class filter_config : boost::noncopyable
{
	
	public:

		void do_flush_redis_ttl_in_unrestrict_cond_config(void);
		std::string do_find_in_fixed_content_config(std::string &key, std::string &value);
		std::string do_find_in_restrict_cond_config(std::string &key, std::string &value);
		std::string do_find_in_unrestrict_cond_config(std::string &key, std::string &value);
		
		int do_filt_time_limit_in_business_config(std::vector<std::string> &vconfigid);
		int do_filt_input_limit(std::map<std::string, std::string> *response_map_param, std::vector<std::string> &vconfigid);
		int do_filt_output_limit(std::map<std::string, std::string> *response_map_param, std::vector<std::string> &vconfigid);

		int do_update_configs(void);
		static void INIT_FILTER_CONFIG(void);

	private:

		std::map<std::string, boost::shared_ptr<fixed_content_response_config> >                         m_fixed_content_config;
		std::map<std::string, boost::shared_ptr<restrict_cond_config > >                                        m_restrict_cond_config;
		std::map<std::string, boost::shared_ptr<business_config> >                                               m_business_config;
		std::map<std::string, boost::shared_ptr<unrestrict_cond_config >  >                                   m_unrestrict_cond_config;

		SDRWLock    m_rwlock;

		static std::string m_table_restrict_cond_config;
		static std::string m_table_unrestrict_cond_config;
		static std::string m_table_business_config;
		static std::string m_table_fixed_content_response_config;

		static int m_time_stamp;
		DECL_LOGGER(logger);
};

#endif   //_FILTER_CONFIG_H


