#include "zmq_client_pub.h"

zmq_client_pub::zmq_client_pub(void)
{
}

zmq_client_pub::~zmq_client_pub(void)
{
}

int zmq_client_pub::init_zmq_client( std::string ip, int port, std::string client_identity, zmq_client_callback func )
{
	return zmq_client::init_zmq_client(ip, port, client_identity, func);

}

int zmq_client_pub::send_data( std::string &data, std::string destination )
{
	return zmq_client::send_data(data,destination);

}
