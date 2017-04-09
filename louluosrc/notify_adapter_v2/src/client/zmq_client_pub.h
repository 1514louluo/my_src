#ifndef 	_SD_ZMQ_CLIENT_PUB_H_
#define 	_SD_ZMQ_CLIENT_PUB_H_

#pragma once

#include "zmq_client.h"

class zmq_client_pub : public zmq_client
{
public:
	zmq_client_pub(void);
	~zmq_client_pub(void);

	int init_zmq_client(std::string ip, int port, std::string client_identity, zmq_client_callback func);

	int send_data(std::string &data, std::string destination);

};
#endif
