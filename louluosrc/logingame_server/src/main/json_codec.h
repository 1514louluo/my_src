#pragma once


#include "mycommon.h"
#include "SDLogger.h"
#include "commondef.h"

class json_codec
{
public:
	json_codec(void);
	~json_codec(void);

	int stu2json_gamelib(STU_GAMELIB_INFO *pstu_info, std::string &str_json);
	int json2stu_gamelib(STU_GAMELIB_INFO *pstu_info, std::string &str_json);
	int stu2json_gamelib_toclient(STU_GAMELIB_INFO *pstu_info, std::string userid, std::string &str_json);
	int json2string_vec_gamelib(std::vector<STU_GAMELIB_INFO> &vec, std::string &str_json);

};
