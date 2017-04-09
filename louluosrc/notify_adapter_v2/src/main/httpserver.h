#pragma once


#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "SDLogger.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "mycommon.h"

#include "block_queue.h"

#include "boost/shared_ptr.hpp"

enum enum_notify_type_http
{
	enum_sock_new_conn_http=0,
	enum_sock_data_recv_http=1,
	enum_sock_close_http=2,
};

#define  HTTP_CHECK_DATA_OK         0
#define  HTTP_CHECK_DATA_NEEDMORE  -1
#define  HTTP_CHECK_DATA_ERROR     -2

#define  PIPE_CMD_SEND           0
#define  PIPE_CMD_CLOSE          1



typedef struct _STU_EPOLL_DATA
{
	int clientfd;
	uint64_t fd_unify_tag;
	sockaddr_in client_socket_addr;
	std::string str_data_recved;
	std::string str_data_write;
}STU_EPOLL_DATA;


typedef int (*httprecv_callback)(enum_notify_type_http notify_type,const char *pdata, int length, 
									STU_EPOLL_DATA *pstu_epoll_data,void *context);



typedef struct _STU_EPOLL_RTN_HTTP
{
	int clientfd;
	uint64_t fd_unify_tag;
	std::string str_data_send;
	int cmd_type;            //PIPE_CMD_SEND   PIPE_CMD_CLOSE
}STU_EPOLL_RTN_HTTP;

class httpserver
{
public:
	httpserver(void);
	~httpserver(void);

	int init_server(int port, httprecv_callback func_callback,int epoll_maxsize, void *context);

	int send_data(int fd, uint64_t unify_tag, std::string &str_data);

	int close_client(int fd, uint64_t unify_tag);

	int get_client_count(void);

	uint64_t get_total_history_client_count(void);

	

private:
	int non_blocking(int socketfd);

	DECL_LOGGER(logger);
	static void *epoll_reactor(void *param);
	void epoll_reactor_doit(void);
	int m_listen_fd;
	int m_port;
	httprecv_callback m_http_callback;
	STU_EPOLL_DATA **m_array_epoll_data;
	int m_epoll_maxsize;
	int m_pipe_read;
	int m_pipe_write;
	int m_pipes[2];
	int m_epoll_fd;
	uint64_t m_unify_id;

	block_queue< boost::shared_ptr<STU_EPOLL_RTN_HTTP> > m_queue_send;

	void close_socket_internal(int clientfd);

	int send_internal(STU_EPOLL_RTN_HTTP *pstu_epoll_rtn);

	int dealwith_recv(int clientfd);

	uint64_t get_unify_tag(int client_fd);

	void *m_context;

	int m_clientcount;

	// 曾经连接上来过的客户端总数
	uint64_t m_client_history_count;

	

	


	

};
