#ifndef 	_SD_ZMQ_CLIENT_SUB_H_
#define 	_SD_ZMQ_CLIENT_SUB_H_


#pragma once

#include "zmq_client.h"

class zmq_client_sub : public zmq_client
{
public:
	zmq_client_sub(void);
	~zmq_client_sub(void);
	int init_zmq_client(std::string ip, int port, std::string client_identity, zmq_client_callback func);

};
#endif
