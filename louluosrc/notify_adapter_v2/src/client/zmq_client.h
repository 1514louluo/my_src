#ifndef 	_SD_ZMQ_CLIENT_H_
#define 	_SD_ZMQ_CLIENT_H_


#pragma once


#include "zhelpers.h"
#include "zmq.hpp"
#include "zmq.h"

#include "mycommon.h"
#include "SDLogger.h"
#include "SDUtility.h"
#include "socket_wrapper.h"
#include "block_queue.h"
typedef boost::function< void (std::string &recv_data) >  zmq_client_callback;


typedef struct _STU_ZMQ_CLIENT_MSG
{
	std::string destination;
	std::string data;
}STU_ZMQ_CLIENT_MSG;

class zmq_client
{
public:
	zmq_client(void);
	~zmq_client(void);

	int init_zmq_client(std::string ip, int port, std::string client_identity, zmq_client_callback func);

	int send_data(std::string &data, std::string destination);


	DECL_LOGGER(logger);
	
private: 
	zmq_client_callback m_callback_notify;
	static void *reactor_proc(void *param);
	void reactor_doit(void);

	static void *send_msg_proc(void *param);
	void send_msg_doit(void);
	
	block_queue< boost::shared_ptr<STU_ZMQ_CLIENT_MSG> > m_queue_send;
	block_queue< boost::shared_ptr<STU_ZMQ_CLIENT_MSG> > m_queue_ipc;
	std::string m_router_ip;
	int m_router_port;
	std::string m_client_ientity;
	std::string m_ipc_addr;
	zmq::socket_t *m_psocket_router;
	zmq::socket_t *m_psocket_cmd;
	static int m_id_inc;

};
#endif
