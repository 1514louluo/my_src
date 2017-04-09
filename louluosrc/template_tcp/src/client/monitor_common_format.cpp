#include "monitor_common_format.h"

monitor_common_format::monitor_common_format(void)
{
	m_is_formated = false;
}

monitor_common_format::~monitor_common_format(void)
{
}

int monitor_common_format::add_kv( std::string key, std::string value )
{
	m_map_kv.insert(make_pair(key, value));
	return 0;

}

int monitor_common_format::get_json_format( std::string &json_data )
{
	if (m_is_formated == true)
	{
		return -1;
	}
	m_is_formated=true;
	cjson_wrapper wrapper;
	wrapper.format_json_fromat(m_map_kv, json_data);
	return 0;

}
