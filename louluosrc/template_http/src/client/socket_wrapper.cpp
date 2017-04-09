#include "socket_wrapper.h"


IMPL_LOGGER(socket_wrapper, logger);
socket_wrapper::socket_wrapper(void)
{
}

socket_wrapper::~socket_wrapper(void)
{
}


int socket_wrapper::recv_try(zmq::socket_t *psock, zmq::message_t *pmsg)
{
	
	int result;
	if (psock == NULL || pmsg == NULL)
	{
		return -1;
	}
	int trycount=0;
	while (true)
	{
		trycount++;
		if (trycount>=100)
		{
			SYS_LOG_ERROR("socket_wrapper::recv_try retry count max");
			return -1;
		}
		result = psock->recv(pmsg);
		if (result == -1)
		{
			if (zmq_errno()==EAGAIN || zmq_errno()==EWOULDBLOCK || zmq_errno()==EINTR)
			{
				SYS_LOG_ERROR("sockets recv  failed "<<trycount<<" times try again, strerr="<<ZMQ_ERROR<<" errno="<<zmq_errno());
				continue;
			}
			return -1;
			
			
		}
		return 0;
	}
	
	/*while (true)
	{	
		uint64_t more=0;
		size_t size=sizeof(more);
		int result;
		result = psock->getsockopt(ZMQ_RCVMORE, &more, &size);
		if (result == -1)
		{
			SYS_LOG_ERROR("sockets getsockopt failed");
			return -1;
		}
		if (more == 0)
		{
			SYS_LOG_ERROR("socket more == 0 error");
			return -1;
		}
	}
	
	
	return 0;*/

}

int socket_wrapper::recv_pack_identity( zmq::socket_t *psocket, STU_ZMQ_INFO_IDENTITY *pstu_info_identity )
{
	if (pstu_info_identity == NULL && psocket == NULL)
	{
		return -1;
	}
	int result;
	zmq::message_t msg_ientity;
	zmq::message_t msg_cmd_type;
	zmq::message_t msg_destination;
	zmq::message_t msg_data;

	result = recv_try(psocket, &msg_ientity);
	if (result == -1)
	{
		SYS_LOG_ERROR("socket get identity failed");
		return -1;
	}
	pstu_info_identity->identity.append((const char *)msg_ientity.data(), msg_ientity.size());

	result = recv_try(psocket, &msg_cmd_type);
	if (result == -1)
	{
		SYS_LOG_ERROR("socket get cmd type failed");
		return -1;
	}
	pstu_info_identity->cmd_type.append((const char *)msg_cmd_type.data(), msg_cmd_type.size());

	result = recv_try(psocket,&msg_destination); 
	if (result == -1)
	{
		SYS_LOG_ERROR("socket get destination failed");
		return -1;
	}
	pstu_info_identity->routing_destination.append((const char *)msg_destination.data(), msg_destination.size());

	result = recv_try(psocket, &msg_data);
	if (result == -1)
	{
		SYS_LOG_ERROR("socket get data failed");
		return -1;
	}
	pstu_info_identity->data.append((const char *)msg_data.data(),msg_data.size());
	return 0;


}

int socket_wrapper::send_pack_identity( zmq::socket_t *psocket, STU_ZMQ_INFO_IDENTITY *pstu_info_identity )
{
	if (psocket == NULL && pstu_info_identity == NULL)
	{
		return -1;
	}
	if (pstu_info_identity->identity.empty())
	{
		SYS_LOG_ERROR("empty identity");
		return -1;
	}
	zmq::message_t msg_ientity((void *)pstu_info_identity->identity.c_str(), pstu_info_identity->identity.size(),NULL,NULL);
	zmq::message_t msg_cmd_type((void *)pstu_info_identity->cmd_type.c_str(),pstu_info_identity->cmd_type.size(),NULL,NULL);
	zmq::message_t msg_destination;
	zmq::message_t msg_data((void *)pstu_info_identity->data.c_str(),pstu_info_identity->data.size(),NULL,NULL);
	//SYS_LOG_DEBUG("identity="<<pstu_info_identity->identity);
	psocket->send(msg_ientity, ZMQ_SNDMORE);
	psocket->send(msg_cmd_type, ZMQ_SNDMORE);
	psocket->send(msg_destination, ZMQ_SNDMORE);
	psocket->send(msg_data);
	return 0;
	

}

int socket_wrapper::recv_pack( zmq::socket_t *psocket, STU_ZMQ_INFO *pstu_info )
{
	if (psocket == NULL && pstu_info == NULL)
	{
		return -1;
	}
	
	zmq::message_t msg_cmd_type;
	zmq::message_t msg_destination;
	zmq::message_t msg_data;
	int result;
	result = recv_try(psocket, &msg_cmd_type);
	if (result == -1)
	{
		SYS_LOG_ERROR("recv cmdtype failed");
		return -1;
	}
	pstu_info->cmd_type.append((const char *)msg_cmd_type.data(), msg_cmd_type.size());

	result = recv_try(psocket, &msg_destination);
	if (result == -1)
	{
		SYS_LOG_ERROR("recv destination failed");
		return -1;
	}
	pstu_info->destination.append((const char *)msg_destination.data(), msg_destination.size());

	result = recv_try(psocket, &msg_data);
	if (result == -1)
	{
		SYS_LOG_ERROR("recv data failed");
		return -1;
	}
	pstu_info->data.append((const char *)msg_data.data(), msg_data.size());
	return 0;



}

int socket_wrapper::send_pack( zmq::socket_t *psocket, STU_ZMQ_INFO *pstu_info )
{
	if (psocket == NULL && pstu_info == NULL)
	{
		return -1;
	}
	if (pstu_info->destination.empty())
	{
		SYS_LOG_ERROR("empty destinnation");
		return -1;
	}
	zmq::message_t msg_cmd_type((void *)pstu_info->cmd_type.c_str(), pstu_info->cmd_type.size(), NULL, NULL);
	zmq::message_t msg_destination((void *)pstu_info->destination.c_str(),pstu_info->destination.size(),NULL,NULL);
	zmq::message_t msg_data((void *)pstu_info->data.c_str(), pstu_info->data.size(), NULL, NULL);
	int result;
	result = psocket->send(msg_cmd_type, ZMQ_SNDMORE);
	if (result == -1)
	{
		SYS_LOG_ERROR("send msg_cmd_type failed, strerr="<<ZMQ_ERROR);
		return -1;
	}
	result = psocket->send(msg_destination,ZMQ_SNDMORE);
	if (result == -1)
	{
		SYS_LOG_ERROR("send destination failed,strerr="<<ZMQ_ERROR);
		return -1;
	}
	result = psocket->send(msg_data);
	if (result == -1)
	{
		SYS_LOG_ERROR("send msg_data failed,strerr="<<ZMQ_ERROR);
		return -1;
	}
	return 0;

}
