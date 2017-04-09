#include "message_codec.h"

#include "SDConfigurationSingleton.h"

IMPL_LOGGER(message_codec, logger);
std::string message_codec::m_verify_key;

message_codec::message_codec(void)
{
}

message_codec::~message_codec(void)
{
}

void message_codec::decode_content( std::string &content )
{
	string2map(content, m_content_map);

}

std::string message_codec::get_content_path( void )
{
	return get_vaule("path", m_content_map);
}

int message_codec::content_auth_verify( void )
{
	
	std::string gameinternalno = get_vaule("gameinternalno", m_content_map);
	//std::string sessionid = get_vaule("sessionid", m_content_map);
	std::string sign = get_vaule("sign", m_content_map);
	std::string key=get_vaule("key", m_content_map);
	if ( sign.empty() || key.empty())
	{
		MY_LOGERROR("empty param content_auth_verify");
		return -1;
	}
	std::string md5before = gameinternalno+key+m_verify_key;
	std::string md5after = SDUtility::get_md5(md5before);
	if (md5after != sign)
	{
		MY_LOGERROR("invalid sign,md5after="<<md5after<<"  sign="<<sign<<"  md5before="<<md5before);
		return -1;
	}
	return 0;


}

void message_codec::INIT_MESSAGE_CODEC( void )
{
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	m_verify_key = config.getString("verify_key_message","123456");


}

int message_codec::encode_auth( bool validation, std::string &content, std::string seqid, std::string tokenid )
{
	std::map<std::string, std::string> map_result;
	map_result.insert(std::make_pair("path", CONTENT_PATH_AUTH_ACK));
	map_result.insert(std::make_pair("seqid", seqid));
	map_result.insert(std::make_pair("tokenid",tokenid));
	if (validation == true)
	{
		map_result.insert(std::make_pair("result","1"));
	} 
	else
	{
		map_result.insert(std::make_pair("result","102"));
	}
	map2string(content, map_result);
	return 0;

}

int message_codec::encode_getkey_rtn( bool validation, std::string &randomkey , std::string &rtn_content)
{
	std::map<std::string, std::string> mapparam;
	mapparam.insert(std::make_pair("path", CONTENT_PATH_GETKEY_ACK));
	if (validation == true)
	{
		mapparam.insert(std::make_pair("result","1"));
		mapparam.insert(std::make_pair("key",randomkey));
	}
	else
	{
		mapparam.insert(std::make_pair("result","3"));
	}
	map2string(rtn_content, mapparam);
	return 0;

}

int message_codec::encode_heartbeat_rtn( std::string &rtn_content )
{
	std::map<std::string, std::string> mapparam;
	mapparam.insert(std::make_pair("path", CONTENT_PATH_HEARTBEAT_ACK));
	map2string(rtn_content, mapparam);
	return 0;

}
