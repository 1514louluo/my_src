#include "zmq_client.h"


IMPL_LOGGER(zmq_client, logger);

int zmq_client::m_id_inc=0;

zmq_client::zmq_client(void)
{
	//m_pcontent = new zmq::context_t(1);
}

zmq_client::~zmq_client(void)
{
}

int zmq_client::init_zmq_client( std::string ip, int port,std::string client_identity, zmq_client_callback func )
{
	srand(time(NULL)+(m_id_inc++));
	std::string random_addr=SDUtility::format((uint64_t)random());
	m_ipc_addr=m_ipc_addr+"ipc:///tmp/"+random_addr;
	m_router_ip = ip;
	m_router_port = port;
	if (client_identity.empty())
	{
		uint64_t myrand =random();
		std::string str_random;
		str_random = SDUtility::format(myrand);
		m_client_ientity = str_random;
	}
	else
	{
		m_client_ientity = client_identity;
	}
	
	if (func== NULL)
	{
		return -1;
	}
	m_callback_notify = func;
	
	pthread_t thrreadid;
	int result;
	result = pthread_create(&thrreadid, NULL, zmq_client::reactor_proc, this);
	if (result == -1)
	{
		MY_LOGERROR("failed to start thread zmq_client::reactor_proc");
		return -1;
	}

	result = pthread_create(&thrreadid, NULL, zmq_client::send_msg_proc, this);
	if (result == -1)
	{
		MY_LOGERROR("failed to start thread zmq_client::send_msg_proc");
		return -1;
	}

	
	return 0;


}

void * zmq_client::reactor_proc( void *param )
{
	if (param == NULL)
	{
		return (void *)NULL;
	}
	zmq_client *pthis=(zmq_client *)param;
	pthis->reactor_doit();
	MY_LOGERROR("zmq_client::reactor_proc terminiated");
	return (void *)NULL;

}

void zmq_client::reactor_doit( void )
{
	MY_LOGDEBUG("zmq_client::reactor_doit started");
	int result;
	zmq::context_t *pcontext_client=new zmq::context_t(1);
	std::string str_connection;
	str_connection = "tcp://"+m_router_ip+":"+SDUtility::format(m_router_port);
	MY_LOGDEBUG("conn="<<str_connection);
	m_psocket_router = new zmq::socket_t(*pcontext_client,ZMQ_DEALER);
	m_psocket_router->setsockopt(ZMQ_IDENTITY, m_client_ientity.c_str(),m_client_ientity.size());
	uint32_t water_value=1000000;
	m_psocket_router->setsockopt(ZMQ_RCVHWM,(void *)&water_value, sizeof(water_value));
	m_psocket_router->setsockopt(ZMQ_SNDHWM,(void *)&water_value, sizeof(water_value));
	result = m_psocket_router->connect(str_connection.c_str());
	if (result == -1)
	{
		MY_LOGERROR("connect to router failed ,conn str="<<str_connection);
		return;
	}

	
	m_psocket_cmd=new zmq::socket_t(*pcontext_client, ZMQ_ROUTER);
	//uint32_t water_value=1000000;
	m_psocket_cmd->setsockopt(ZMQ_RCVHWM,(void *)&water_value, sizeof(water_value));
	m_psocket_cmd->setsockopt(ZMQ_SNDHWM,(void *)&water_value, sizeof(water_value));
	result = m_psocket_cmd->bind(m_ipc_addr.c_str());
	MY_LOGDEBUG("ipc="<<m_ipc_addr);
	if (result == -1)
	{
		MY_LOGERROR("failed to bind socket to ipc,ipc="<<m_ipc_addr);
		return;
	}

	zmq_pollitem_t items [] =
	{
		{ m_psocket_router->get(), 0, ZMQ_POLLIN, 0 },
		{ m_psocket_cmd->get(),  0, ZMQ_POLLIN, 0 },
		
	};

	time_t updatetime=time(NULL);
	socket_wrapper sockwrapper;
	while (true)
	{
		int result;
		zmq::message_t message;
		int poll_result = zmq_poll (items, 2, 1000);
		if (poll_result == -1)
		{
			MY_LOGERROR("failed to zmq_poll in dispatch_transport_doit,errstr="<<ZMQ_ERROR<<" errno="<<zmq_errno());
			continue;
		}
		time_t currenttime = time(NULL);
		if (currenttime>updatetime)
		{
			time_t diff=currenttime-updatetime;
			if (diff>=5)
			{
				updatetime=time(NULL);
				STU_ZMQ_INFO stu_heartbeat;
				stu_heartbeat.cmd_type=ZMQ_CMD_TYPE_HEARTBEAT;
				stu_heartbeat.data="unkown";
				stu_heartbeat.destination="unkown";
				sockwrapper.send_pack(m_psocket_router, &stu_heartbeat);
				MY_LOGDEBUG("ientity="<<m_client_ientity<<" send heartbeat finished");
			}
		}
		
		if (items [0].revents & ZMQ_POLLIN)    // router, send package to  client
		{
			STU_ZMQ_INFO stu;
			result = sockwrapper.recv_pack(m_psocket_router,&stu);
			
			if (result == -1)
			{
				continue;
			}
			if (stu.cmd_type == ZMQ_CMD_TYPE_DATA)
			{
				m_callback_notify(stu.data);
			}
			else if(stu.cmd_type == ZMQ_CMD_TYPE_HEARTBEAT)
			{
				MY_LOGDEBUG("identity="<<m_client_ientity<<"  recv heartbeat, cmdtype="<<stu.cmd_type);
			}

		}

		

		if (items [1].revents & ZMQ_POLLIN)  // ipc send package to reactor
		{
			//MY_LOGDEBUG("recv ipc message");
			STU_ZMQ_INFO_IDENTITY stu_zmq_info_identity;
			result = sockwrapper.recv_pack_identity(m_psocket_cmd, &stu_zmq_info_identity);
			if (result == -1)
			{
				MY_LOGERROR("sockwrapper.recv_pack_identity error");
				continue;
			}
			boost::shared_ptr<STU_ZMQ_CLIENT_MSG> pstu_msg;
			result = m_queue_ipc.dequeue_nodelay(pstu_msg);
			if (result != 1)
			{
				MY_LOGERROR("m_queue_ipc.dequeue_nodelay error");
				continue;
			}

			
			STU_ZMQ_INFO stu;
			stu.data = pstu_msg->data;
			stu.cmd_type=ZMQ_CMD_TYPE_DATA;
			stu.destination = pstu_msg->destination;
			//MY_LOGDEBUG("data="<<stu.data<<" dst="<<stu.destination);
			result = sockwrapper.send_pack(m_psocket_router, &stu);
			if (result == -1)
			{
				MY_LOGERROR("failed to send msg to router");
			}

		}


	}






}




int zmq_client::send_data( std::string &data, std::string destination )
{
	boost::shared_ptr<STU_ZMQ_CLIENT_MSG> stu(new STU_ZMQ_CLIENT_MSG);
	stu->data = data;
	stu->destination = destination;
	m_queue_send.add_queue(stu);
	return 0;

}

void * zmq_client::send_msg_proc( void *param )
{
	if (param == NULL)
	{
		return (void *)param;
	}
	zmq_client *pthis = (zmq_client *)param;
	pthis->send_msg_doit();
	MY_LOGERROR("zmq_client::send_msg_proc terminated");
	return (void *)NULL;

}

void zmq_client::send_msg_doit( void )
{
	MY_LOGDEBUG("send_msg_doit thread start");
	int result;
	sleep(1);
	zmq::context_t *pcontext = new zmq::context_t(1);
	zmq::socket_t *psocket_cmd = new zmq::socket_t(*pcontext, ZMQ_DEALER);
	uint32_t water_value=1000000;
	psocket_cmd->setsockopt(ZMQ_RCVHWM,(void *)&water_value, sizeof(water_value));
	psocket_cmd->setsockopt(ZMQ_SNDHWM,(void *)&water_value, sizeof(water_value));
	result = psocket_cmd->connect(m_ipc_addr.c_str());
	if (result == -1)
	{
		MY_LOGERROR("failed to connect to ipc, addr ="<<m_ipc_addr);
		return;
	}

	while (true)
	{
		
		boost::shared_ptr<STU_ZMQ_CLIENT_MSG> pstu;
		result = m_queue_send.dequeue_expand(pstu);
		if (result != 1)
		{
			continue;
		}
		//MY_LOGDEBUG("recv msg from m_queue_send, data="<<pstu->data<<" dst="<<pstu->destination);
		m_queue_ipc.add_queue(pstu);
		STU_ZMQ_INFO stu;
		stu.cmd_type=ZMQ_CMD_TYPE_DATA;
		stu.data = pstu->data;
		stu.destination = pstu->destination;
		socket_wrapper wrapper;
		wrapper.send_pack(psocket_cmd, &stu);




	}

}

//int zmq_client::direct_send( std::string data, std::string dst )
//{
//
//	zmq::socket_t sock(*m_pcontent, ZMQ_DEALER);
//	sock.connect("tcp://127.0.0.1:4444");
//	while (true)
//	{
//		socket_wrapper wrapper;
//		STU_ZMQ_INFO stu;
//		stu.cmd_type="data";
//		stu.destination="xxxx";
//		stu.data="content";
//		wrapper.send_pack(&sock, &stu);
//		printf("direct_send\n");
//		sleep(5);
//
//	}
//}
