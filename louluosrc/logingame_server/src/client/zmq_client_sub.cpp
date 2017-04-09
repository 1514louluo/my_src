#include "zmq_client_sub.h"

zmq_client_sub::zmq_client_sub(void)
{
}

zmq_client_sub::~zmq_client_sub(void)
{
}

int zmq_client_sub::init_zmq_client( std::string ip, int port, std::string client_identity, zmq_client_callback func )
{
	return zmq_client::init_zmq_client(ip,port, client_identity, func);
}

