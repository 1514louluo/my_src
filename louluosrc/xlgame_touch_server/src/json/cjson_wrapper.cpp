#include "cjson_wrapper.h"

cjson_wrapper::cjson_wrapper(std::string &str_json)
{
	m_pjsonobj = cJSON_Parse(str_json.c_str());
}

cjson_wrapper::cjson_wrapper( void )
{
	m_pjsonobj=NULL;

}

cjson_wrapper::~cjson_wrapper(void)
{
	if (m_pjsonobj != NULL)
	{
		cJSON_Delete(m_pjsonobj);
		m_pjsonobj = NULL;
	}
}

int cjson_wrapper::get_string( std::string key, std::string &value )
{
	if (m_pjsonobj == NULL)
	{
		return -1;
	}
	cJSON *pobj=NULL;
	pobj = cJSON_GetObjectItem(m_pjsonobj, key.c_str());
	if (pobj)
	{
		switch( pobj->type)
		{
			case cJSON_String: 
				value = pobj->valuestring;
				break;
			case cJSON_Number:
				value = SDUtility::format(pobj->valueint);
				break;
			default:
				break;
		}
	}
	return 0;
}

int cjson_wrapper::get_int( std::string key, int &value )
{
	if (m_pjsonobj == NULL)
	{
		return -1;
	}
	cJSON *pobj=NULL;
	pobj = cJSON_GetObjectItem(m_pjsonobj, key.c_str());
	if(pobj)
		value = pobj->valueint;
	return 0;

}

int cjson_wrapper::format_json_fromat( std::map<std::string, std::string> &mapparam, std::string &str_json )
{
	cJSON *pjsonobj = cJSON_CreateObject();
	std::map<std::string, std::string>::iterator iter;
	for (iter=mapparam.begin(); iter != mapparam.end(); iter++)
	{
		cJSON_AddStringToObject(pjsonobj, iter->first.c_str(), iter->second.c_str());
	}
	char *pdata = cJSON_Print(pjsonobj);
	str_json = pdata;
	free(pdata);
	cJSON_Delete(pjsonobj);
	return 0;

}

int cjson_wrapper::format_json_unfromat( std::map<std::string, std::string> &mapparam, std::string &str_json )
{
	cJSON *pjsonobj = cJSON_CreateObject();
	std::map<std::string, std::string>::iterator iter;
	for (iter=mapparam.begin(); iter != mapparam.end(); iter++)
	{
		cJSON_AddStringToObject(pjsonobj, iter->first.c_str(), iter->second.c_str());
	}
	char *pdata = cJSON_PrintUnformatted(pjsonobj);
	str_json = pdata;
	free(pdata);
	cJSON_Delete(pjsonobj);
	return 0;

}

