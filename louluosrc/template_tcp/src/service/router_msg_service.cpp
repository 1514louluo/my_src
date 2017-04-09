#include "router_msg_service.h"

router_msg_service::router_msg_service(void)
{
}

router_msg_service::~router_msg_service(void)
{
}

void router_msg_service::adapter_to_notify_server_encode_json( std::string msgoriginal,
							std::string from, std::string gameinternalno, std::string &content, std::string &str_json )
{
	cjson_wrapper wrapper;
	std::map<std::string, std::string> mapparam;
	mapparam.insert(make_pair("msgoriginal",msgoriginal));
	mapparam.insert(make_pair("from",from));
	mapparam.insert(make_pair("gameinternalno",gameinternalno));
	mapparam.insert(make_pair("content",content));

	wrapper.format_json_unfromat(mapparam, str_json);

	/*
	{��msgoriginal��:��cli��,     ��ʶ����ԭʼ��Դ�ڿͻ���
	��from��:����̨�������ơ�,    ��ʶ��·�ɷ�����Ϣ�ķ�������ƣ����ڻظ�ʱ���·�ɶ�λ
	��gameinternalno��:��0937367283648��,  ��ʶ��Ӧ�Ŀͻ���gameinternalno��ʶ���������˷������Ϣ�����Ը��ֶ�
	��content��:��͸������������յ�����Ϣ����path=����}  �������Ϣ����

	
	*/
}
