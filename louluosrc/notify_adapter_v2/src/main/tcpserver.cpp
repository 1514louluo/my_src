#include "tcpserver.h"

#include "socket_client.h"
IMPL_LOGGER(tcpserver, logger);

tcpserver::tcpserver(void)
{
	m_unify_id=0;
	m_clientcount=0;

	// 曾经连接上来过的客户端总数
	m_client_history_count=0;

	m_sendbuf = 1024*30; 
	m_sendbufsize=sizeof(uint32_t); 
}

tcpserver::~tcpserver(void)
{
}


uint64_t tcpserver::get_unify_tag( int client_fd )
{
	// 高32位fd，低32位为自增长id
	m_unify_id++;
	uint64_t unifyid = m_unify_id;
	unifyid = unifyid & 0xFFFFFFFF;
	uint64_t fd64  = client_fd;
	fd64 = fd64 << 32;
	uint64_t rtn = fd64 + unifyid;
	return rtn;

}

int tcpserver::non_blocking(int socketfd)
{

	int flags, s;

	flags = fcntl (socketfd, F_GETFL, 0);
	if (flags == -1)
	{
		//perror ("fcntl");
		MY_LOGERROR("failed to get socket non blocking");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl (socketfd, F_SETFL, flags);
	if (s == -1)
	{
		MY_LOGERROR("failed to set socket non blocking");
		return -1;
	}

	return 0;
}


void * tcpserver::reactor_proc( void *param )
{
	if (param == NULL)
	{
		return (void *)NULL;
	}
	tcpserver *pthis = (tcpserver *)param;
	pthis->reactor_doit();
	MY_LOGERROR("tcpserver::reactor_proc terminated failed");
	return (void *)NULL;


}

void tcpserver::reactor_doit( void )
{
	MY_LOGINFO("ready to start  tcpserver");
	int result = 0;

	m_epoll_maxsize = 200000;
	if (m_callback == NULL)
	{
		return;
	}

	// 监听端口
	m_listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int reuse = 1;
	if (setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) 
	{
		MY_LOGERROR("set socket reuse failed");
		return;
	}

	char *ip_address = "0.0.0.0";
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_address);
	addr.sin_port = htons(m_port);

	if (bind(m_listen_fd, (const struct sockaddr*)&addr, (socklen_t)sizeof(addr)) == -1) 
	{
		MY_LOGERROR("failed to bind socket fd to ip and port="<<m_port);
		return;
	}

	if (listen(m_listen_fd, SOMAXCONN)== -1)
	{
		MY_LOGERROR("failed to listen on port="<<m_port);
		return;
	}

	result = pipe(m_pipes);
	if (result == -1)
	{
		MY_LOGERROR("failed to create epoll pipes");
		return;
	}
	m_pipe_read = m_pipes[0];
	m_pipe_write = m_pipes[1];


	
	
	int epoll_fd = epoll_create(m_epoll_maxsize);
	m_epoll_fd = epoll_fd;
	if(epoll_fd == -1)
	{
		MY_LOGERROR("epoll_fd == -1");
		return;
	}
	
	m_array_epoll_data = (void **)new char[sizeof(void *) * m_epoll_maxsize];
	for (int i=0; i<m_epoll_maxsize; i++)
	{
		m_array_epoll_data[i] = NULL;
	}

	// add listen fd to epoll
	epoll_event event_listen;
	event_listen.data.fd = m_listen_fd;
	event_listen.events = EPOLLIN;
	result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_listen_fd, &event_listen);
	if (result == -1)
	{
		MY_LOGERROR("failed to add listen fd to epoll fd");
		return;
	}

	// add pipe fd to epoll
	epoll_event event_pipe;
	event_pipe.data.fd = m_pipe_read;
	event_pipe.events = EPOLLIN;
	result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_pipe_read, &event_pipe);
	if (result == -1)
	{
		MY_LOGERROR("failed to add pipe fd to epoll fd");
		return;
	}

	epoll_event *pepoll_events = new epoll_event[m_epoll_maxsize];

	// 开始reactor的循环
	while (true)
	{

		int fds = epoll_wait(epoll_fd, pepoll_events, m_epoll_maxsize, m_epoll_timeout_seconds*1000);
		
		if (fds == 0)
		{
			MY_LOGINFO("epoll wait timeout notify");
			check_timeout();
			continue;
		}

		if (fds == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) 
			{
				continue;
			}
			MY_LOGERROR("epoll wait error, errno="<<errno<<" errstr="<<strerror(errno));
			return;
		}


		int i;
		for (i=0; i<fds; i++)
		{
			// check if new client connection
			if (pepoll_events[i].data.fd == m_listen_fd)  // 新的客户端连接过来了
			{
				dealwith_new_connection(m_listen_fd);
				continue;
			}

			if (pepoll_events[i].data.fd == m_pipe_read)  // pipe管道有数据了
			{
				dealwith_pipe_cmd(m_pipe_read);
				continue;

			}
			
			int recv_result = dealwith_recv(pepoll_events[i].data.fd);
			if (recv_result == -1)
			{
				close_internal(pepoll_events[i].data.fd);
			}
			
			
			

		}

		check_timeout();
		
		



	}
 
 
}

int tcpserver::init_tcpserver( type_notify_callback callback, type_new_connect_callback new_connectcbk,
							  type_get_length_callback getheadlengthcbk,
							  type_get_length_callback getbodylengthcbk)
{
	if (callback == NULL || new_connectcbk == NULL || getbodylengthcbk==NULL || getheadlengthcbk == NULL)
	{
		return -1;
	}
	m_last_timeout_check_seconds=time(NULL);
	m_callback = callback;
	m_callback_new_client = new_connectcbk;
	m_get_head_length_callback = getheadlengthcbk;
	m_get_body_length_callback = getbodylengthcbk;
	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	m_port = config.getInt("notify_adapter_server_port", 0);
	m_epoll_timeout_seconds = config.getInt("epoll_timeout_seconds",5);
	m_check_timeout_interval_seconds = config.getInt("check_timeout_interval_seconds", 100);
	m_invalid_socket_timeout_seconds = config.getInt("invalid_socket_timeout_seconds",100);
	int result;
	pthread_t threadid;
	result = pthread_create(&threadid, NULL, tcpserver::reactor_proc, this);
	if (result == -1)
	{
		MY_LOGERROR("failed to create thread tcpserver::reactor_proc");
		return -1;
	}




	return 0;

}

int tcpserver::dealwith_new_connection( int listenfd )
{
	struct sockaddr_in new_addr;
	memset(&new_addr, 0, sizeof(new_addr));
	socklen_t new_addr_len = sizeof(new_addr);
	int new_client_fd;
	new_client_fd = accept(listenfd, (struct sockaddr*)&new_addr, &new_addr_len);
	if (new_client_fd == -1)
	{
		MY_LOGERROR("failed to accept new client from listen fd, errstr="<<strerror(errno));
		return -1;
	}
	int noblock_result = non_blocking(new_client_fd);
	if (noblock_result == -1)
	{
		MY_LOGERROR("failed to set new client fd to nonblocking");
		return -1;
	}
	// set sendbuffer;

	int optresult = setsockopt(new_client_fd,SOL_SOCKET,SO_SNDBUF,(char*) &m_sendbuf,m_sendbufsize); 
	if (optresult == -1)
	{
		MY_LOGERROR("failed to set send buffer size,errno="<<errno<<" errstr="<<strerror(errno));
		return -1;
	}
	// add new client fd to epoll fd
	epoll_event new_client_event;
	new_client_event.data.fd = new_client_fd;
	new_client_event.events = EPOLLIN;
	int add_result = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, new_client_fd, &new_client_event);
	if (add_result == -1)
	{
		MY_LOGERROR("failed to add new client fd to epoll fd, errno="<<errno<<" errstr="<<strerror(errno));
		return -1;
	}


	/*STU_EPOLL_DATA *pstu_epoll_data = new STU_EPOLL_DATA;
	pstu_epoll_data->clientfd = new_client_fd;
	pstu_epoll_data->client_socket_addr = new_addr;
	pstu_epoll_data->fd_unify_tag = get_unify_tag(new_client_fd);
	m_array_epoll_data[new_client_fd] = pstu_epoll_data;
	m_http_callback(enum_sock_new_conn, NULL, 0, pstu_epoll_data, m_context);*/
	//int getpeername(int s, struct sockaddr *name, socklen_t *namelen);
	sockaddr_in sevAddr;
	int result;
	socklen_t socklen = sizeof(sevAddr);
	result = getpeername(new_client_fd, (sockaddr *)&sevAddr, &socklen);
	std::string str_clientip=inet_ntoa(sevAddr.sin_addr);
	uint64_t unify_tag = get_unify_tag(new_client_fd);
	MY_LOGDEBUG("new connection, fd="<<new_client_fd<<" tag="<<unify_tag<<"  ip="<<str_clientip);
	void *pclientdata = m_callback_new_client(new_client_fd, unify_tag);
	((socket_client *)pclientdata)->m_clientip = str_clientip;
	m_array_epoll_data[new_client_fd]=pclientdata;
	m_clientcount++;
	m_client_history_count++;

	return 0;


}

int tcpserver::dealwith_pipe_cmd( int pipe_read_fd )
{
	int result;
	char datachar;
	result = read(pipe_read_fd,&datachar, 1);
	if (result != 1)
	{
		MY_LOGERROR("failed to read cmd from pipe");
		return -1;
	}

	STU_EPOLL_RTN stu_epoll_rtn;
	result = m_queue_rtn.dequeue_nodelay(stu_epoll_rtn);
	if (result != 1)
	{
		MY_LOGERROR("failed to fetch send/close cmd data from queue");
		return -1;
	}
	//MY_LOGDEBUG("m_queue_rtn recv stu, fd="<<stu_epoll_rtn.clientfd<<"  tag="<<stu_epoll_rtn.fd_unify_tag
	//	<<" type="<<stu_epoll_rtn.cmd_type<<"  identity="<<stu_epoll_rtn.socket_ientity);
	STU_SOCKET_INFO stu_socket_info;
	std::map<std::string, STU_SOCKET_INFO>::iterator iter;

	// 如果是针对auth类型的命令，不需要查找identity
	if (stu_epoll_rtn.cmd_type == PIPE_CMD_AUTH_OK)
	{
		stu_socket_info.clientfd = stu_epoll_rtn.clientfd;
		stu_socket_info.fd_unify_tag = stu_epoll_rtn.fd_unify_tag;
	}
	else
	{
		if (stu_epoll_rtn.socket_ientity.empty())
		{
			stu_socket_info.clientfd = stu_epoll_rtn.clientfd;
			stu_socket_info.fd_unify_tag = stu_epoll_rtn.fd_unify_tag;
		} 
		else
		{
			iter = m_map_socket_ientity.find(stu_epoll_rtn.socket_ientity);
			if (iter == m_map_socket_ientity.end())
			{
				MY_LOGERROR("send data to terminal, socket identity not avaliable, identity="<<stu_epoll_rtn.socket_ientity);
				return -1;
			}
			stu_socket_info = iter->second;
		}
	}

	if (stu_epoll_rtn.cmd_type == PIPE_CMD_BOADCAST)
	{
		boadcast_internal(stu_epoll_rtn.str_data_send);
		return 0;
	}
	
	
	//STU_SOCKET_INFO stu_socket_info = iter->second;
	if (stu_socket_info.clientfd >= 200000)
	{
		MY_LOGERROR("invalid stu_socket_info.clientfd >= 200000 ");
		return -1;
	}

	socket_client *pclient = (socket_client *)m_array_epoll_data[stu_socket_info.clientfd];
	if (pclient == NULL)
	{
		MY_LOGERROR("pclient == NULL  socket already closed, fd="<<stu_socket_info.clientfd
			<<"  tag="<<stu_socket_info.fd_unify_tag);
		return -1;
	}
	if (pclient->m_fd != stu_socket_info.clientfd  || pclient->m_unify_tag != stu_socket_info.fd_unify_tag)
	{
		MY_LOGERROR("diffent fd or tag, drop send data or close");
		return -1;
	}


	switch (stu_epoll_rtn.cmd_type)
	{
	case PIPE_CMD_SEND:
		{
			pclient->m_last_update_seconds = time(NULL);
			if (stu_epoll_rtn.socket_ientity.empty())
			{
				senddata_internal(stu_epoll_rtn.str_data_send, stu_socket_info.clientfd);
			}
			else
			{
				std::map<std::string, STU_SOCKET_INFO>::iterator iter;
				iter = m_map_socket_ientity.find(stu_epoll_rtn.socket_ientity);
				if (iter == m_map_socket_ientity.end())
				{
					MY_LOGERROR("terminal client not connected yet, socket_ientity="<<stu_epoll_rtn.socket_ientity);
					return 0;
				}
				senddata_internal(stu_epoll_rtn.str_data_send,iter->second.clientfd);

			}
			
			return 0;
		}
		break;
	case PIPE_CMD_CLOSE:
		{
			close_internal(stu_socket_info.clientfd);
			return 0;

		}
		break;
	case PIPE_CMD_AUTH_OK:
		{
			// 
			MY_LOGDEBUG("deal PIPE_CMD_AUTH_OK pipe cmd");
			socket_client *pclient = (socket_client *)m_array_epoll_data[stu_epoll_rtn.clientfd];
			if (pclient == NULL)
			{
				MY_LOGERROR("PIPE_CMD_AUTH_OK failed, empty socket client ,pclient == NULL");
				return 0;
			}
			if (pclient->m_fd != stu_epoll_rtn.clientfd  || pclient->m_unify_tag != stu_epoll_rtn.fd_unify_tag)
			{
				MY_LOGERROR("PIPE_CMD_AUTH_OK failed, empty socket client ,diff fd or tag");
				return 0;
			}
			if (stu_epoll_rtn.socket_ientity.empty())
			{
				MY_LOGERROR("empty ientity with PIPE_CMD_AUTH_OK");
				return 0;
			}
			pclient->m_socket_identity = stu_epoll_rtn.socket_ientity;
			pclient->m_is_authed=true;
			STU_SOCKET_INFO stu;
			stu.clientfd = stu_epoll_rtn.clientfd;
			stu.fd_unify_tag = stu_epoll_rtn.fd_unify_tag;

			// 需要验证该socket identity是否已经连接上了
			std::map<std::string, STU_SOCKET_INFO>::iterator iter;
			iter = m_map_socket_ientity.find(stu_epoll_rtn.socket_ientity);
			if (iter == m_map_socket_ientity.end())
			{
				MY_LOGDEBUG("insert sock_identity-->fd,tag info, sock_ientity="<<stu_epoll_rtn.socket_ientity<<
					"  fd="<<stu_epoll_rtn.clientfd<<"  tag="<<stu_epoll_rtn.fd_unify_tag);
				m_map_socket_ientity.insert(make_pair(stu_epoll_rtn.socket_ientity, stu));
			} 
			else
			{
				// 需要处理这种情况，即：如果是同一个连接，连续发送2个auth命令，都验证通过的时候，需要先确认
				// 该sock_identity对应的fd是否是同一个
				if (stu_epoll_rtn.clientfd == iter->second.clientfd)
				{
					MY_LOGDEBUG("exist same sock_identity, and same fd, means same tcp link, more auth cmd");
				} 
				else
				{
					MY_LOGDEBUG("exist same sock_ientity, but not same fd, close the first tcp link");
					close_internal(iter->second.clientfd);
					m_map_socket_ientity.insert(make_pair(stu_epoll_rtn.socket_ientity, stu));
				}
				
				

			}
			MY_LOGDEBUG("auth socket ok, identity="<<stu_epoll_rtn.socket_ientity<<" fd="<<stu_epoll_rtn.clientfd
						<<"  tag="<<stu_epoll_rtn.fd_unify_tag);
			
			
		}
		break;
	default:
		MY_LOGERROR("unknown cmd type");
		return -1;
	}


	return 0;

}

int tcpserver::senddata_internal( std::string &data_send, int fd )
{
	MY_LOGDEBUG("senddata_internal fd="<<fd<<"  length="<<data_send.size());
	int result = send(fd, data_send.c_str(), data_send.size(), MSG_DONTWAIT);

	if (result == -1)  // 这个逻辑分支需要专门处理，针对几种情况
	{
		close_internal(fd);
		return -1;
	}
	if (result != data_send.size())
	{
		MY_LOGERROR("client terminal error, not enough buffer for send data, should close socket");
		return -1;
	}

	return 0;

}

int tcpserver::close_internal( int fd )
{
	
	epoll_event event;
	event.data.fd = 0;
	event.events = 0;
	int del_result = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &event);
	if (del_result == -1)
	{
		MY_LOGERROR("failed to del client fd from epoll fd,errno="<<errno<<"  strerr="<<strerror(errno));
	}
	close(fd);
	
	m_clientcount--;
	socket_client *pclient = (socket_client *)m_array_epoll_data[fd];
	m_array_epoll_data[fd] = NULL;
	if (pclient != NULL)
	{
		m_callback(enum_sock_close, pclient);
		if (! pclient->m_socket_identity.empty())
		{
			m_map_socket_ientity.erase(pclient->m_socket_identity);
		}
		delete pclient;
	}
	else
	{
		MY_LOGERROR("ERROR pclient == NULL, skip delete pclient");
	}
	return -1;
	

}

int tcpserver::dealwith_recv( int fd )
{
	int read_count=0;
	socket_client *pclient = (socket_client *)m_array_epoll_data[fd];
	if (pclient == NULL)
	{
		MY_LOGERROR("fd client not malloc");
		return -1;
	}
	int result;
	int headlength;
	headlength = m_get_head_length_callback(m_array_epoll_data[fd]);
	if (headlength == -1)
	{
		return -1;
	}
	int size_recved = pclient->m_data_recv.size();
	if (size_recved < headlength)
	{   // 接收报头
		int size_needto_recv = headlength-size_recved;
		char headdata[40]={0};
		result = read(fd, headdata, size_needto_recv);
		read_count++;   // 一次event事件响应，只允许有一次read，否则可能会导致reactor线程挂起
		if (result == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) 
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		if (result == 0)  // 对端关闭连接
		{
			return -1;  // 在函数外层关闭
		}
		pclient->m_data_recv.append(headdata, result);
		if (result < size_needto_recv)
		{
			// 表示报头还没有收完，需要继续收报头
			return 0;
		}
	}
	if (read_count>0)  // 一次event事件响应，只允许有一次read，否则可能会导致reactor线程挂起
	{
		return 0;
	}
	// 接收body
	int body_length = m_get_body_length_callback(pclient);
	if (body_length == -1)
	{ // 说明解析head部分的具体字段有误，可能是攻击，需要关闭socket
		std::string strprint;
		printstring2str(pclient->m_data_recv, strprint);
		MY_LOGERROR("invalid head bytes content, str="<<strprint);
		return -1;
	}
	char charbody[4096]={0};
	int body_needto_recved = body_length-(pclient->m_data_recv.size()-headlength);
	if (body_needto_recved > 4096)
	{
		body_needto_recved=4096;
	}
	result = recv(fd, charbody,body_needto_recved, MSG_DONTWAIT);
	if (result == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) 
		{
			return 0;
		}
		else
		{
			return -1;
		}

	}
	if (result == 0)
	{   // 对端关闭连接
		return -1;   // 在函数外层关闭
	}

	pclient->m_data_recv.append(charbody, result);

	if (result<body_needto_recved)  // 还需要增加读取更多的body数据
	{
		MY_LOGDEBUG("more body data needed");
		return 0;
	}

	result = m_callback(enum_sock_data_recv, pclient);
	if (result != TCP_CHECK_OK)
	{
		return -1;
	}
	pclient->m_data_recv.clear();

	return 0;





}



int tcpserver::close_socket( int fd, uint64_t tag )
{
	STU_EPOLL_RTN stu_rtn;
	stu_rtn.clientfd = fd;
	stu_rtn.fd_unify_tag = tag;
	stu_rtn.cmd_type = PIPE_CMD_CLOSE;
	m_queue_rtn.add_queue(stu_rtn);
	// 写入pipe通知
	return write_byte_to_pipe();
	
	

}

int tcpserver::socket_auth_ok( int fd, uint64_t tag, std::string &socket_identity )
{
	STU_EPOLL_RTN stu;
	stu.clientfd=fd;
	stu.fd_unify_tag = tag;
	stu.socket_ientity = socket_identity;
	stu.cmd_type = PIPE_CMD_AUTH_OK;
	//MY_LOGDEBUG("tcpserver::socket_auth_ok, fd="<<fd<<" tag="<<tag<<"  identity="<<socket_identity);
	m_queue_rtn.add_queue(stu);
	write_byte_to_pipe();
	return 0;

}

int tcpserver::write_byte_to_pipe( void )
{
	int result;
	const char chardata=0;
	bool is_write_success=false;
	for (int i=0; i<=5; i++)
	{
		result = write(m_pipe_write, &chardata,1);
		if (result == -1)
		{
			MY_LOGERROR("write to pipe faied, errono="<<errno<<"  strerr="<<strerror(errno));
			continue;
		}
		is_write_success=true;
		break;
	}

	if (is_write_success == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

int tcpserver::send_data( int fd, uint64_t tag, std::string &senddata )
{
	STU_EPOLL_RTN stu;
	stu.clientfd =fd;
	stu.fd_unify_tag = tag;
	stu.str_data_send = senddata;
	stu.cmd_type = PIPE_CMD_SEND;
	m_queue_rtn.add_queue(stu);
	return write_byte_to_pipe();

}

int tcpserver::send_data_ientity( std::string socket_identity, std::string &senddata )
{
	STU_EPOLL_RTN stu;
	stu.clientfd =0;
	stu.fd_unify_tag = 0;
	stu.socket_ientity = socket_identity;
	stu.str_data_send = senddata;
	stu.cmd_type=PIPE_CMD_SEND;
	m_queue_rtn.add_queue(stu);
	return write_byte_to_pipe();

}

int tcpserver::check_timeout( void )
{
	// 后面需要进行超时检测
	uint64_t currentseconds = time(NULL);
	
	if (currentseconds>m_last_timeout_check_seconds)
	{
		uint64_t diff = currentseconds - m_last_timeout_check_seconds;
		//MY_LOGDEBUG("currenttime="<<currentseconds<<"　 lasttime="<<m_last_timeout_check_seconds<<"  diff="<<diff);
		if (diff >= m_check_timeout_interval_seconds)
		{
			m_last_timeout_check_seconds = currentseconds;
			for (int fd=0; fd<200000; fd++)
			{
				socket_client *pclient = (socket_client *)(m_array_epoll_data[fd]);
				if (pclient != NULL)
				{
					if (pclient->m_last_update_seconds < currentseconds)
					{
						uint64_t diff_fd = currentseconds-pclient->m_last_update_seconds;
						if (diff_fd>m_invalid_socket_timeout_seconds)
						{
							MY_LOGDEBUG("fd timeout, close,fd="<<pclient->m_fd);
							close_internal(pclient->m_fd);
						}
						/*else
						{
							MY_LOGDEBUG("difftime="<<diff_fd);
						}*/
					}

				}
			}
		}
	}

}

int tcpserver::get_current_conn( void )
{
	return m_clientcount;

}

uint64_t tcpserver::get_conn_history( void )
{
	return m_client_history_count;

}

int tcpserver::boadcast( std::string boadcastinfo )
{
	MY_LOGDEBUG("ready to commit boadcast");
	STU_EPOLL_RTN stu;
	stu.clientfd =0;
	stu.fd_unify_tag = 0;
	//stu.socket_ientity = socket_identity;
	stu.str_data_send = boadcastinfo;
	stu.cmd_type=PIPE_CMD_BOADCAST;
	m_queue_rtn.add_queue(stu);
	return write_byte_to_pipe();

}

int tcpserver::boadcast_internal( std::string boadcastdata )
{
	MY_LOGDEBUG("tcpserver::boadcast_internal");
	int boadcast_success=0;
	int boadcast_failed=0;
	int result;
	std::vector<int> vec_fd_ready2close;
	std::map<std::string, STU_SOCKET_INFO>::iterator iter;
	for (iter = m_map_socket_ientity.begin();  iter != m_map_socket_ientity.end(); iter++)
	{
		
		int fd;
		fd = iter->second.clientfd;
		int result = send(fd, boadcastdata.c_str(), boadcastdata.size(), MSG_DONTWAIT);
		MY_LOGDEBUG("boadcast to fd="<<iter->second.clientfd<<"  tag="<<iter->second.fd_unify_tag);

		if (result == -1)  // 这个逻辑分支需要专门处理，针对几种情况
		{
			MY_LOGERROR("boadcast_internal send data failed");
			vec_fd_ready2close.push_back(fd);
			boadcast_failed++;
			continue;
		}
		if (result != boadcastdata.size())
		{
			MY_LOGERROR("boadcast_internal client terminal error, not enough buffer for send data, should close socket");
			vec_fd_ready2close.push_back(fd);
			boadcast_failed++;
			continue;
		}
		boadcast_success++;
	}

	BOOST_FOREACH(int fd, vec_fd_ready2close)
	{
		close_internal(fd);
	}
	MY_LOGDEBUG("implement boadcast, success="<<boadcast_success<<"  failed="<<boadcast_failed);
	return 0;

}
