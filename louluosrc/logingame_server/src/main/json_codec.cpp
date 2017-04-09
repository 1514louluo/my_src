#include "json_codec.h"

#include "cJSON.h"
 
json_codec::json_codec(void)
{
}

json_codec::~json_codec(void)
{
}


int json_codec::stu2json_gamelib( STU_GAMELIB_INFO *pstu_info, std::string &str_json )
{
	

	if (pstu_info == NULL)
	{
		return -1;
	}
	cJSON *json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "gameinternalno", pstu_info->gameinternalno.c_str());
	cJSON_AddStringToObject(json, "gameid", pstu_info->gameid.c_str());
	cJSON_AddStringToObject(json, "serverid", pstu_info->serverid.c_str());
	cJSON_AddStringToObject(json, "sid", pstu_info->sid.c_str());

	char *pchar = cJSON_PrintUnformatted(json);
	str_json = pchar;
	free(pchar);

	cJSON_Delete(json);
	return 0;


}

int json_codec::json2string_vec_gamelib( std::vector<STU_GAMELIB_INFO> &vec, std::string &str_json )
{

	return 0;
}

int json_codec::json2stu_gamelib( STU_GAMELIB_INFO *pstu_info, std::string &str_json )
{
	if (pstu_info == NULL)
	{
		return -1;
	}
	cJSON *json = cJSON_Parse(str_json.c_str());
	if (json == NULL)
	{
		return -1;
	}
	cJSON *pitem = NULL;
	pitem = cJSON_GetObjectItem(json, "gameinternalno");
	if (pitem != NULL)
	{
		pstu_info->gameinternalno = pitem->valuestring;
	}

	pitem = cJSON_GetObjectItem(json, "gameid");
	if (pitem != NULL)
	{
		pstu_info->gameid = pitem->valuestring;
	}

	pitem = cJSON_GetObjectItem(json, "serverid");
	if (pitem != NULL)
	{
		pstu_info->serverid = pitem->valuestring;
	}

	pitem = cJSON_GetObjectItem(json, "sid");
	if (pitem != NULL)
	{
		pstu_info->sid = pitem->valuestring;
	}

	cJSON_Delete(json);

	return 0;


}

int json_codec::stu2json_gamelib_toclient( STU_GAMELIB_INFO *pstu_info, std::string userid, 
										  std::string &str_json )
{
	if (pstu_info == NULL)
	{
		return -1;
	}
	cJSON *json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "userid", userid.c_str());
	cJSON_AddStringToObject(json, "gameid", pstu_info->gameid.c_str());
	cJSON_AddStringToObject(json, "serverid", pstu_info->serverid.c_str());
	cJSON_AddStringToObject(json, "sid", pstu_info->sid.c_str());

	char *pchar = cJSON_PrintUnformatted(json);
	str_json = pchar;
	free(pchar);

	cJSON_Delete(json);
	return 0;

}
