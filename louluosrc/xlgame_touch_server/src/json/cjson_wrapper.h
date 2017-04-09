#pragma once

#include "cJSON.h"
#include "mycommon.h"
#include "SDLogger.h"
#include "SDUtility.h"

class cjson_wrapper
{
public:
	cjson_wrapper(std::string &str_json);
	cjson_wrapper(void);
	~cjson_wrapper(void);

	int get_string(std::string key, std::string &value);

	int get_int(std::string key, int &value);

	int format_json_fromat(std::map<std::string, std::string> &mapparam, std::string &str_json);
	int format_json_unfromat(std::map<std::string, std::string> &mapparam, std::string &str_json);

private:
	cJSON *m_pjsonobj;
};
