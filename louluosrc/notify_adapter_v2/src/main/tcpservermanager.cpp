#include "tcpservermanager.h"
#include "service_stat.h"
IMPL_LOGGER(tcpservermanager, logger);

#include "dispatcher.h"

tcpservermanager::tcpservermanager(void)
{
}

tcpservermanager::~tcpservermanager(void)
{
}

int tcpservermanager::init_tcpserver_manager( void )
{
	message_codec::INIT_MESSAGE_CODEC();
	type_notify_callback notify_callback;
	notify_callback = boost::bind(&tcpservermanager::data_notify_callback, this, _1, _2);

	type_new_connect_callback new_connect_callback;
	new_connect_callback = boost::bind(&tcpservermanager::new_connect_callback, this, _1, _2);

	type_get_length_callback get_headlength_callback;
	type_get_length_callback get_bodylength_callback;

	get_headlength_callback = boost::bind(&tcpservermanager::get_head_length,this, _1);
	get_bodylength_callback = boost::bind(&tcpservermanager::get_body_length,this, _1);



	m_tcpserver.init_tcpserver(notify_callback, new_connect_callback, get_headlength_callback,
								get_bodylength_callback);

	boost::function<void (void)> timercallback = boost::bind(&tcpservermanager::get_stat_timer_callback, this);
	m_timer_get_stat.startimer(timercallback, 1);
	MY_LOGDEBUG("tcpservermanager::init_tcpserver_manager finished");
	return 0;
}

int tcpservermanager::data_notify_callback(enum_notify_type notify_type, void *pclientdata )
{
	switch (notify_type)
	{
	case enum_sock_data_recv:
		{
			if (pclientdata == NULL)
			{
				MY_LOGERROR("pclientdata == NULL");
				return TCP_CHECK_ERROR;
			}
			socket_client *pclient = (socket_client *)pclientdata;
			// 需要做数据包验证
			int result;
			protocol_codec codec;
			std::string content;
			result = codec.get_body((char *)pclient->m_data_recv.c_str(), pclient->m_data_recv.size(), content);
			if (result == -1)
			{
				MY_LOGERROR("codec.get_body failed");
				return TCP_CHECK_ERROR;
			}
			//std::string content;
			//result = codec.get_body_string(str_body, content);
			//if (result == -1)
			//{
			//	MY_LOGERROR("codec.get_body_string failed");
			//	return TCP_CHECK_ERROR;
			//}
			

			// 做message的解码
			message_codec msgcodec;
			msgcodec.decode_content(content);
			std::string contentpath=msgcodec.get_content_path();


			// 完成一整包的数据包接收，需要auth，并提交请求
			if (pclient->m_is_authed == false)
			{	// 未进行权限验证
				if (contentpath == CONTENT_PATH_AUTH)
				{
					// 检查编码是否正确
					int auth_result = msgcodec.content_auth_verify();
					if (auth_result == -1)
					{
						MY_LOGERROR("content_auth_verify  failed");
						return TCP_CHECK_ERROR;
					}
				}
				else if(contentpath == CONTENT_PATH_GETKEY)
				{
					// 获取key
				}
				else 
				{
					// 其他类型，未验证，认为是攻击，直接关闭
					MY_LOGERROR("pclient->m_is_authed == false, content="<<content);
					return TCP_CHECK_ERROR;
				}
				
			}

			if (contentpath == CONTENT_PATH_HEARTBEAT)
			{
				MY_LOGDEBUG("recv heartbeat from fd="<<pclient->m_fd<<" tag="<<pclient->m_unify_tag);
				protocol_codec pro_codec1;
				std::string rtn_content;
				msgcodec.encode_heartbeat_rtn(rtn_content);
				std::string rtn_buffer;
				pro_codec1.encode_buffer(0, rtn_content,rtn_buffer);
				reply_data(pclient->m_fd, pclient->m_unify_tag, rtn_buffer);
				// 对于心跳，仍然需要通知给上层应用
				dispatcher *pdispatcher = (dispatcher *)m_parent;
				boost::shared_ptr<STU_MSG_INFO> stu_msg_info(new STU_MSG_INFO);
				stu_msg_info->fd = pclient->m_fd;
				stu_msg_info->tag = pclient->m_unify_tag;
				stu_msg_info->content = content;
				stu_msg_info->tokenid = pclient->m_socket_identity;
				stu_msg_info->content_path = CONTENT_PATH_HEARTBEAT;
				pdispatcher->m_queue_adapter_cmd.add_queue(stu_msg_info);
				return TCP_CHECK_OK;
			}

			// 将消息向处理线程丢过去
			dispatcher *pdispatcher = (dispatcher *)m_parent;
			boost::shared_ptr<STU_MSG_INFO> stu_msg_info(new STU_MSG_INFO);
			stu_msg_info->fd = pclient->m_fd;
			stu_msg_info->tag = pclient->m_unify_tag;
			stu_msg_info->content = content;
			stu_msg_info->tokenid = pclient->m_socket_identity;
			stu_msg_info->content_path = msgcodec.get_content_path();
			pdispatcher->m_queue_adapter_cmd.add_queue(stu_msg_info);
			return TCP_CHECK_OK;

		}
		break;
	case enum_sock_close:
		{
			socket_client *pclient = (socket_client *)pclientdata;
			MY_LOGDEBUG("socket closed, fd="<<pclient->m_fd<<"  tag="<<pclient->m_unify_tag);
			// 当客户端离线的时候，如果是已经认证了的连接，且有tokenid，则需要发送离线通知
			if (! pclient->m_socket_identity.empty())
			{
				dispatcher *pdispatcher = (dispatcher *)m_parent;
				boost::shared_ptr<STU_MSG_INFO> stu_msg_info(new STU_MSG_INFO);
				stu_msg_info->fd = pclient->m_fd;
				stu_msg_info->tag = pclient->m_unify_tag;
				//stu_msg_info->content = content;
				stu_msg_info->tokenid = pclient->m_socket_identity;
				stu_msg_info->content_path = CONTENT_PATH_OFFLINE;
				pdispatcher->m_queue_adapter_cmd.add_queue(stu_msg_info);
			}
			return TCP_CHECK_OK;
		}
		break;
	}

	return TCP_CHECK_OK;


}

void * tcpservermanager::new_connect_callback( int newfd, uint64_t unify_tag )
{
	//MY_LOGDEBUG("new connect fd="<<newfd<<"　tag="<<unify_tag);
	//MY_LOGDEBUG("new connect fd="<<newfd<<"  tag="<<unify_tag);
	socket_client *pclient = new socket_client;
	pclient->m_fd = newfd;
	pclient->m_unify_tag = unify_tag;
	return (void *)pclient;
	

}

int tcpservermanager::get_head_length( void *pclientdata )
{
	protocol_codec codec;
	return codec.get_head_length();

}

int tcpservermanager::get_body_length( void *pclientdata )
{
	socket_client *pclient = (socket_client *)pclientdata;
	protocol_codec codec;
	int result;
	result = codec.get_body_length((char *)pclient->m_data_recv.c_str(), pclient->m_data_recv.size());
	return result; 


}

void tcpservermanager::setparent( void *param )
{
	m_parent = param;

}

int tcpservermanager::reply_data( int fd, uint64_t tag, std::string &strdata )
{
	m_tcpserver.send_data(fd, tag, strdata);
	return 0;

}

int tcpservermanager::reply_data_identity( std::string sock_ientity, std::string &strdata )
{
	m_tcpserver.send_data_ientity(sock_ientity, strdata);
	return 0;

}

int tcpservermanager::reply_close( int fd, uint64_t tag )
{
	m_tcpserver.close_socket(fd, tag);
	return 0;

}

int tcpservermanager::reply_close_identity( std::string sock_identity )
{
	//m_tcpserver.cl
	return 0;

}

int tcpservermanager::reply_auth_ok(int fd, uint64_t tag,  std::string sock_identity )       
{
	/*STU_EPOLL_RTN stu;
	stu.clientfd = fd;
	stu.fd_unify_tag = tag;
	stu.socket_ientity = sock_identity;
	stu.str_data_send = senddata;*/
	m_tcpserver.socket_auth_ok(fd, tag, sock_identity);
	return 0;

}

void tcpservermanager::get_stat_timer_callback( void )
{
	//MY_LOGDEBUG("tcpservermanager::get_stat_timer_callback,"<<m_tcpserver.get_current_conn()<<"  "
	//	<<m_tcpserver.get_conn_history());
	service_stat *pstat = service_stat::singleton();
	pstat->m_tcp_curent_conn = m_tcpserver.get_current_conn();
	pstat->m_tcp_history_conn = m_tcpserver.get_conn_history();

}

int tcpservermanager::boadcast( std::string strdata )
{
	m_tcpserver.boadcast(strdata);
	return 0;

}












