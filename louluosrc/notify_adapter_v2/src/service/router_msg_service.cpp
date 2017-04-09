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
	{“msgoriginal”:”cli”,     标识请求原始来源于客户端
	“from”:”后台服务名称”,    标识向路由发送消息的服务的名称，用于回复时候的路由定位
	”gameinternalno”:”0937367283648”,  标识对应的客户端gameinternalno标识，如果服务端发起的消息，忽略该字段
	”content”:”透传接入服务器收到的消息，如path=…”}  具体的消息内容

	
	*/
}
