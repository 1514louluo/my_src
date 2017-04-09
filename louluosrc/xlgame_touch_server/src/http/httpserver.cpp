#include "httpserver.h"


IMPL_LOGGER(httpserver, logger)

httpserver::httpserver(void)
{
	m_port = 0;
	m_listen_fd = 0;
	m_http_callback = NULL;
	m_array_epoll_data = NULL;
	m_pipe_read = 0;
	m_pipe_write = 0;
	m_epoll_maxsize = 60000;
	m_unify_id = 0;
	m_clientcount=0;
	m_client_history_count = 0;
}

httpserver::~httpserver(void)
{
}

int httpserver::init_server( int port, httprecv_callback func_callback, int epoll_maxsize , void *context)
{
	MY_LOGINFO("ready to start  httpserver");
	int result = 0;
	m_port = port;
	
	m_epoll_maxsize = epoll_maxsize;
	if (func_callback == NULL)
	{
		return -1;
	}
	m_context = context;
	m_http_callback = func_callback;

	// 监听端口
	m_listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int reuse = 1;
	if (setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) 
	{
		MY_LOGERROR("set socket reuse failed");
		return -1;
	}

	char *ip_address = "0.0.0.0";
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_address);
	addr.sin_port = htons(port);

	if (bind(m_listen_fd, (const struct sockaddr*)&addr, (socklen_t)sizeof(addr)) == -1) 
	{
		MY_LOGERROR("failed to bind socket fd to ip and port="<<m_port);
		return -1;
	}

	if (listen(m_listen_fd, SOMAXCONN)== -1)
	{
		MY_LOGERROR("failed to listen on port="<<m_port);
		return -1;
	}

	result = pipe(m_pipes);
	if (result == -1)
	{
		MY_LOGERROR("failed to create epoll pipes");
		return -1;
	}
	m_pipe_read = m_pipes[0];
	m_pipe_write = m_pipes[1];

	m_array_epoll_data = (STU_EPOLL_DATA **)new char[sizeof(STU_EPOLL_DATA *) * m_epoll_maxsize];
	for (int i=0; i<m_epoll_maxsize; i++)
	{
		m_array_epoll_data[i] = NULL;
	}

	pthread_t threadid;
	result = pthread_create(&threadid, NULL, httpserver::epoll_reactor, this);
	if (result == -1)
	{
		MY_LOGERROR("failed to create http server epoll read send thread");
		return -1;
	}

	MY_LOGINFO("box httpserver start finished");

	return 0;

}

void * httpserver::epoll_reactor( void *param )
{

	if (param == NULL)
	{
		return (void *)0;
	}

	httpserver *pthis = (httpserver *)param;
	pthis->epoll_reactor_doit();

	MY_LOGINFO("epoll_reator thread terminated");
	return (void *)0;
}

int httpserver::non_blocking(int socketfd)
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

void httpserver::epoll_reactor_doit( void )
{

	//
	uint32_t sendbuf = 1024*30; 
	uint32_t sendbufsize=sizeof(uint32_t); 
	int result = 0;
	int epoll_fd = epoll_create(m_epoll_maxsize);
	m_epoll_fd = epoll_fd;
	if(epoll_fd == -1)
	{
		return;
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

	// start epollwait loop
	while (true)
	{
		
		int fds = epoll_wait(epoll_fd, pepoll_events, m_epoll_maxsize, 50000);
		if (fds == 0)
		{
			MY_LOGINFO("epoll wait timeout notify");
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
			if (pepoll_events[i].data.fd == m_listen_fd)
			{
				struct sockaddr_in new_addr;
				memset(&new_addr, 0, sizeof(new_addr));
				socklen_t new_addr_len = sizeof(new_addr);
				int new_client_fd;
				new_client_fd = accept(m_listen_fd, (struct sockaddr*)&new_addr, &new_addr_len);
				if (new_client_fd == -1)
				{
					MY_LOGERROR("failed to accept new client from listen fd, errstr="<<strerror(errno));
					continue;
				}
				int noblock_result = non_blocking(new_client_fd);
				if (noblock_result == -1)
				{
					MY_LOGERROR("failed to set new client fd to nonblocking");
					continue;
				}
				// set sendbuffer;

				int optresult = setsockopt(new_client_fd,SOL_SOCKET,SO_SNDBUF,(char*) &sendbuf,sendbufsize); 
				if (optresult == -1)
				{
					MY_LOGERROR("failed to set send buffer size,errno="<<errno<<" errstr="<<strerror(errno));
					continue;
				}
				// add new client fd to epoll fd
				epoll_event new_client_event;
				new_client_event.data.fd = new_client_fd;
				new_client_event.events = EPOLLIN;
				int add_result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_client_fd, &new_client_event);
				if (add_result == -1)
				{
					MY_LOGERROR("failed to add new client fd to epoll fd");
					continue;
				}

				
				STU_EPOLL_DATA *pstu_epoll_data = new STU_EPOLL_DATA;
				pstu_epoll_data->clientfd = new_client_fd;
				pstu_epoll_data->client_socket_addr = new_addr;
				pstu_epoll_data->fd_unify_tag = get_unify_tag(new_client_fd);
				m_array_epoll_data[new_client_fd] = pstu_epoll_data;
				m_http_callback(enum_sock_new_conn, NULL, 0, pstu_epoll_data, m_context);
				m_clientcount++;
				m_client_history_count++;
				continue;
			}

			if (pepoll_events[i].data.fd == m_pipe_read)
			{
				// new pipe cmd arrive
				char pipe_data;
				int pipe_result = read(m_pipe_read, &pipe_data, 1);
				if (pipe_result == -1)
				{
					MY_LOGERROR("get cmd from pipe read failed, continue");
					continue;
				}

				boost::shared_ptr<STU_EPOLL_RTN> stu_epoll_rtn;
				int queue_result = m_queue_send.dequeue_nodelay(stu_epoll_rtn);
				if (queue_result != 1)
				{
					MY_LOGERROR("failed to get data from queue");
					continue;
				}
				
				// check if this socket is closed already
				if (m_array_epoll_data[stu_epoll_rtn->clientfd] == NULL)
				{
					MY_LOGERROR("socket already closed, drop send or close cmd");
					continue;
				}

				// check fd validation
				if (m_array_epoll_data[stu_epoll_rtn->clientfd]->clientfd == stu_epoll_rtn->clientfd
					&& m_array_epoll_data[stu_epoll_rtn->clientfd]->fd_unify_tag == stu_epoll_rtn->fd_unify_tag)
				{
					
				}
				else
				{
					MY_LOGERROR("different clientfd and unify tag, skip close or send");
					continue;
				}

				


				switch (stu_epoll_rtn->cmd_type)
				{
				case PIPE_CMD_SEND:
					{
						int send_result = send_internal(stu_epoll_rtn.get());
						if (send_result == -1)
						{
							close_socket_internal(stu_epoll_rtn->clientfd);
						}
						
					}
					break;
				case PIPE_CMD_CLOSE:
					{
						//MY_LOGDEBUG("ready to close socket from pipe cmd");
						close_socket_internal(stu_epoll_rtn->clientfd);
					}
					break;
				default:
					break;
				}
				continue;
			}



			int recv_result = dealwith_recv(pepoll_events[i].data.fd);
			if (recv_result == -1)
			{
				close_socket_internal(pepoll_events[i].data.fd);
				
			}

			continue;



			
		}



	}

}

int httpserver::send_internal(STU_EPOLL_RTN *pstu_epoll_rtn)
{
	int result = send(pstu_epoll_rtn->clientfd, pstu_epoll_rtn->str_data_send.c_str(), 
					pstu_epoll_rtn->str_data_send.size(), MSG_DONTWAIT);
	
	if (result == -1)
	{
		return -1;
	}
	if (result != pstu_epoll_rtn->str_data_send.size())
	{
		MY_LOGERROR("client terminal error, not enough buffer for send data, should close socket");
		return -1;
	}
	
	return 0;
	
}

int httpserver::dealwith_recv(int clientfd)
{
	//  check if the application layer buffer is avaliable
	if (m_array_epoll_data[clientfd] == NULL)
	{
		return -1;
	}
	char char_buffer[4096] = {0};
	int recv_result = recv(clientfd, char_buffer, 4096, MSG_DONTWAIT);
	if (recv_result == 0)
	{
		// client closed
		return -1;
	}

	if (recv_result == -1)
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

	STU_EPOLL_DATA *pstu_epolldata = m_array_epoll_data[clientfd];
	pstu_epolldata->str_data_recved.append(char_buffer, recv_result);
	int notify_result = m_http_callback(enum_sock_data_recv,pstu_epolldata->str_data_recved.c_str(), 
										pstu_epolldata->str_data_recved.size(),
										pstu_epolldata, m_context);
	
	switch (notify_result)
	{
	case HTTP_CHECK_DATA_OK:
		pstu_epolldata->str_data_recved.clear();
		return 0;
		break;
	case HTTP_CHECK_DATA_NEEDMORE:
		return 0;
		break;
	case HTTP_CHECK_DATA_ERROR:
		{
			MY_LOGERROR("http request error");
			return -1;
		}
		
		break;
	default:
		MY_LOGERROR("unknown notify result type");
		return 0;
		break;
	}

	return 0;




}

void httpserver::close_socket_internal( int clientfd )
{

	// 
	epoll_event event;
	event.data.fd = 0;
	event.events = 0;
	int del_result = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, clientfd, &event);
	if (del_result == -1)
	{
		MY_LOGERROR("failed to del client fd from epoll fd");
	}
	close(clientfd);
	
	m_clientcount--;
	
	if (m_array_epoll_data[clientfd] != NULL)
	{
		m_http_callback(enum_sock_close, NULL, 0, m_array_epoll_data[clientfd], m_context);
		delete m_array_epoll_data[clientfd];
		
		m_array_epoll_data[clientfd] = NULL;
	}
	
	

}

uint64_t httpserver::get_unify_tag( int client_fd )
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

int httpserver::send_data( int fd, uint64_t unify_tag, std::string &str_data )
{
	boost::shared_ptr<STU_EPOLL_RTN> pstu_rtn(new STU_EPOLL_RTN);
	pstu_rtn->clientfd = fd;
	pstu_rtn->fd_unify_tag = unify_tag;
	pstu_rtn->str_data_send = str_data;
	pstu_rtn->cmd_type = PIPE_CMD_SEND;
	
	m_queue_send.add_queue(pstu_rtn);

	char charnotify = 0;
	int pipe_result = write(m_pipe_write, &charnotify, 1);
	if (pipe_result == -1)
	{
		MY_LOGERROR("failed to pipe cmd send ");
		return -1;
	}

	return 0;

}

int httpserver::close_client( int fd, uint64_t unify_tag )
{
	boost::shared_ptr<STU_EPOLL_RTN> pstu_rtn(new STU_EPOLL_RTN);
	pstu_rtn->clientfd = fd;
	pstu_rtn->fd_unify_tag = unify_tag;
	pstu_rtn->cmd_type = PIPE_CMD_CLOSE;

	m_queue_send.add_queue(pstu_rtn);

	char charnotify = 0;
	int pipe_result = write(m_pipe_write, &charnotify, 1);
	if (pipe_result == -1)
	{
		MY_LOGERROR("failed to pipe cmd close ");
		return -1;
	}

	return 0;

}

int httpserver::get_client_count( void )
{

	return m_clientcount;
}

uint64_t httpserver::get_total_history_client_count( void )
{

	return m_client_history_count;
}


