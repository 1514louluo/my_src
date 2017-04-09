#include "msg_codec.h"

IMPL_LOGGER(msg_codec, logger);

msg_codec::msg_codec(void){}
msg_codec::~msg_codec(void){}


bool msg_codec::setData(const std::string &sKey,  const std::string &sVal)
{
	bool ret = insert_map(sKey, sVal);

	return ret;

}

bool msg_codec::setData(const char  *sKey,  const std::string  &sVal)
{
	std::string key = sKey;
	bool ret = insert_map(key, sVal);

	return ret;
}

std::string msg_codec::getSendData(void)
{
	if(m_send_data.empty())
	{
		encode_info();
	}
	return m_send_data;
}


static int format_json_unfromat(std::map<std::string, std::string> &mapparam, std::string &str_json)
{
        cJSON *pjsonobj = cJSON_CreateObject();
        std::map<std::string, std::string>::iterator iter;
        for(iter=mapparam.begin(); iter != mapparam.end(); iter++)
        {
                cJSON_AddStringToObject(pjsonobj, iter->first.c_str(), iter->second.c_str());
        }
        char *pdata = cJSON_PrintUnformatted(pjsonobj);
        str_json = pdata;
        free(pdata);
        cJSON_Delete(pjsonobj);
        return 0;
}

bool msg_codec::encode_info()
{
	if(!m_data_map.empty())
	{
		format_json_unfromat(m_data_map, m_send_data);
	}

	return true;
}

bool msg_codec::insert_map(const std::string& sKey,  const std::string& sVal)
{

	std::map<std::string, std::string>::iterator iter = m_data_map.find(sKey);
	if(iter != m_data_map.end())// ´æÔÚ
	{
		iter->second = sVal;
	}
	else//²»´æÔÚ
	{
		m_data_map.insert(make_pair(sKey, sVal));
	}

	return true;
}












