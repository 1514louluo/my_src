#include "zmq_client_pub_client.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "cJSON.h"
#include "SDConfiguration.h"
#include "SDConfigurationSingleton.h"
#include "mycommon.h"
using namespace std;

IMPL_LOGGER(zmq_client_pub_Singleton, logger);

IMPL_LOGGER(Router_coder, logger);


///////////////////zmq_client_pub_client class 实现//////////


zmq_client_pub_Singleton* zmq_client_pub_Singleton::m_instance = new zmq_client_pub_Singleton();

bool zmq_client_pub_Singleton::init_router_info(void)
{
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	m_router_ip = config.getString("router_pub_ip", "10.10.1.58");
	m_router_pub_port = config.getInt("router_pub_port", 4444);
 	m_destination_identity = config.getString("destination_identity", "log_server");
	m_self_identity = config.getString("self_identity","service");
	m_self_identity = m_self_identity + SDUtility::format((uint64_t)time(NULL));

	LOG4CPLUS_INFO(logger, "initialize router_info SUCCESS!  router_pub_ip:" << m_router_ip  << " router_pub_port:" << m_router_pub_port << " router_identity: " << m_destination_identity);
	return true;
}


int zmq_client_pub_Singleton::init(void )
{
	//初始化路由信息
	init_router_info();
	
	boost::function<void (std::string &) > myfunc=boost::bind(&zmq_client_pub_Singleton::callback_recv, this, _1);
	
	int ret = m_zmq_client_pub.init_zmq_client(m_router_ip, m_router_pub_port, m_self_identity, myfunc);
	
	return ret ;
}



int zmq_client_pub_Singleton::commit(std::string& sendData)
{
	MY_LOGDEBUG("destination:"<<m_destination_identity<<"  send logdata:"<<sendData);
	return m_zmq_client_pub.send_data(sendData, m_destination_identity);
}





///////////////////Router_coder class 实现//////////
Router_coder::Router_coder(void)
{
	bIsEncode = false;
	bIsDecode = false;
	bDecodeError = false;
	bIsEncode_to_json = false;
}

Router_coder::~Router_coder(void)
{

}

void Router_coder::setBaseinfo(const std::string& server_name, const std::string& server_ip, const std::string& service_name )
{
	m_baseinfo.server_name = server_name;
	m_baseinfo.server_ip = server_ip;
	m_baseinfo.service_name = service_name;

	//插入map中
	setData(SERVER_NAME_KEY,m_baseinfo.server_name);
	setData(SERVER_IP_KEY,m_baseinfo.server_ip);
	setData(SERVICE_NAME_KEY,m_baseinfo.service_name);
	
	//LOG4CPLUS_INFO(logger, "setBaseinfo SUCCESS! server_name:" << m_baseinfo.server_name  << " service_name:" << m_baseinfo.service_name << " server_ip: " << m_baseinfo.server_ip << " data_map.size: " <<m_data_map.size());
	
}

bool Router_coder::setData(const std::string sKey,  const std::string sVal)
{
	bool ret = insert_map(sKey, sVal);

	return ret;

}


bool Router_coder::setData(const std::string sKey,  const int iVal)
{
	std::ostringstream val;
	val<<iVal;

	bool ret = insert_map(sKey, val.str());
	
	val.str("");

	return ret;

}

bool Router_coder::setData(const std::string sKey,  const long lVal)
{
	std::ostringstream val;
	val<<lVal;

	bool ret = insert_map(sKey, val.str());
	
	val.str("");

	return ret;

}

bool Router_coder::getSendData(std::string & destData)
{
	if(bIsEncode)
	{
		destData = m_send_data;
	}
	else
	{
		encode_info();
		destData = m_send_data;
	}
	return true;
}





////private  函数 实现/////

//编码
bool Router_coder::encode_info()
{
	if(bIsEncode)
	{
		MY_LOGERROR("failed to Router_coder::encode_info! the data is encoded! ");
		return false;
	}
	
	if(0 == m_data_map.size())
	{
		MY_LOGERROR("failed to Router_coder::encode_info! data_map.size = " << m_data_map.size());
		return  false;
	}
	
	cJSON *jsonobj = cJSON_CreateObject();
	
	string key,val;
	
	std::map<string, string>::iterator iter = m_data_map.begin();
	for(; iter != m_data_map.end(); ++iter)
	{
		key = iter->first;
		val = iter->second;
		cJSON_AddStringToObject(jsonobj, key.c_str(), val.c_str());
	}
	

	char *pcharjson = cJSON_PrintUnformatted(jsonobj);
	m_send_data = pcharjson;
	free(pcharjson);
	cJSON_Delete(jsonobj);

	bIsEncode = true;
	
	LOG4CPLUS_INFO(logger, "encode_info SUCCESS!  send_data:" << m_send_data);

	return true;
}




bool Router_coder::encode_baseinfo_to_json()
{
	cJSON *jsonobj = cJSON_CreateObject();
	
	string key,val;
	
	//server_name
	key = SERVER_NAME_KEY;
	val = m_baseinfo.server_name;
	cJSON_AddStringToObject(jsonobj, key.c_str(), val.c_str());
	
	//service_name
	key = SERVICE_NAME_KEY;
	val = m_baseinfo.service_name;
	cJSON_AddStringToObject(jsonobj, key.c_str(), val.c_str());
	

	char *pcharjson = cJSON_PrintUnformatted(jsonobj);
	m_baseinfo_to_json = pcharjson;
	free(pcharjson);
	cJSON_Delete(jsonobj);

	bIsEncode = true;
	
	LOG4CPLUS_INFO(logger, "encode_info SUCCESS! info_to_json:" << m_baseinfo_to_json );

	return true;

}



bool Router_coder::insert_map(const std::string& sKey,  const std::string& sVal)
{

	std::map<string, string>::iterator iter = m_data_map.find(sKey);
	if(iter != m_data_map.end())// 存在
	{
		iter->second = sVal;
	}
	else//不存在
	{
		m_data_map.insert(map< std::string,  std::string> :: value_type(sKey, sVal));
	}

	return true;
}












