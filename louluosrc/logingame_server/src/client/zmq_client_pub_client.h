#ifndef 	_SD_ZMQ_CLIENT_PUB_CLIENT_H_
#define 	_SD_ZMQ_CLIENT_PUB_CLIENT_H_

#define SERVER_NAME_KEY  "SERVER_NAME" 
#define SERVICE_NAME_KEY  "SERVICE_NAME" 
#define SERVER_IP_KEY  "SERVER_IP" 

#pragma once


#include "zmq_client.h"
#include "zmq_client_sub.h"
#include "zmq_client_pub.h"
#include "SDPthreadLock.h"
#include <map>
#include <vector>
#include <string>
#include "SDLogger.h"



using namespace std;


typedef struct baseinfo
{
	std::string server_name;//������
	std::string server_ip;//������ip
	std::string service_name;//ҵ����
}BASEINFO;

class Router_coder
{
public:
	Router_coder(void);
	~Router_coder(void);
	
	//encoder function out  ������ʹ��
	void setBaseinfo(const std::string & server_name, const std::string & server_ip, const std::string& service_name);
	bool setData(const std::string  sKey,  const std::string  sVal);
	bool setData(const std::string  sKey,  const int  iVal);
	bool setData(const std::string  sKey,  const long  lVal);
	bool getSendData(std::string & destData);
	

	
	
private:
	////decoder function out  ������ʹ��
	//bool decode_baseinfo(std::string recv_data);
	//bool getBaseinfo_to_json(std::string & destData);
	//bool setKeyArrays( std::vector<std::string>* keyArrays);
	//bool addtoKeyArrays(const std::string& sKey);
	//bool decode_info(std::string  recv_data);
	//bool getBaseinfo(BASEINFO & baseinfo);
	//bool format_valArrays_to_log(std::ostringstream & destlog, const std::string & ROW_COMPOSE_CHAR);
	//void getLogLock(boost::shared_ptr<SDLock>&  log_lock);
	//void setLogLock(boost::shared_ptr<SDLock>  log_lock);
	bool encode_info();
	bool encode_baseinfo_to_json();
	bool insert_map(const std::string& sKey,  const std::string& sVal);

private:
	//���ݽṹ
	BASEINFO m_baseinfo;
	std::string m_baseinfo_to_json;

	//send data
	std::map<std::string, std::string> m_data_map;
	std::string m_send_data;
	bool bIsEncode;

	//recv data :key - val arrays
	std::vector<std::string> m_key_arrays;
	std::vector<std::string> m_val_arrays;
	std::string m_recv_data;
	bool bIsDecode;
	bool bDecodeError;
	bool bIsEncode_to_json;

	//д�����ʱ����Ӧ�ļ����ļ���
	//boost::shared_ptr<SDLock>  m_log_lock;
	DECL_LOGGER(logger);
};


//����ģʽ�������߳̿����˷�
class zmq_client_pub_Singleton 
{
public:
	
	~zmq_client_pub_Singleton(void)
	{

	}
	
	int init();
	int commit( std::string & sendData);
	static zmq_client_pub_Singleton* get_instance()
	{
		return m_instance;
	}
	
protected: 
	   zmq_client_pub_Singleton(){m_router_pub_port = 0;}
	   zmq_client_pub_Singleton(const zmq_client_pub_Singleton&){}
	   zmq_client_pub_Singleton& operator=(const zmq_client_pub_Singleton&){return *this;}

private:
	bool init_router_info();
	void callback_recv(std::string &data)
	{
		//����ʵ��
	}
	
private:
	//router����
	zmq_client_pub m_zmq_client_pub;
	
	//routerģ����Ϣ :�����ļ�
	std::string m_router_ip;
	int m_router_pub_port;
	std::string m_destination_identity;
	std::string m_self_identity;

	//��������
	static zmq_client_pub_Singleton* m_instance;

	DECL_LOGGER(logger);
};


#endif 

