#pragma once
#include "mycommon.h"
#include "zmq.hpp"
#include "zhelpers.h"
#include "commondef.h"
#include "logdef.h"


class socket_wrapper
{
public:
	socket_wrapper(void);
	~socket_wrapper(void);

	int recv_pack_identity(zmq::socket_t *psocket, STU_ZMQ_INFO_IDENTITY *pstu_info_identity);
	int send_pack_identity(zmq::socket_t *psocket, STU_ZMQ_INFO_IDENTITY *pstu_info_identity);

	int recv_pack(zmq::socket_t *psocket, STU_ZMQ_INFO *pstu_info);
	int send_pack(zmq::socket_t *psocket, STU_ZMQ_INFO *pstu_info);

private:
	DECL_LOGGER(logger);

	

	int recv_try(zmq::socket_t *psock, zmq::message_t *pmsg);
};
