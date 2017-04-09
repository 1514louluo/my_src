#pragma once

#include "mycommon.h"
#include "logdef.h"
#include "cjson_wrapper.h"

class monitor_common_format
{
public:
	monitor_common_format(void);
	~monitor_common_format(void);
	int add_kv(std::string key, std::string value);

	int get_json_format(std::string &json_data);

private:
	std::string m_str_json;
	bool m_is_formated;
	std::map<std::string, std::string> m_map_kv;
};
