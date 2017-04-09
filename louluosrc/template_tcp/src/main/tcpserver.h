#pragma once

#include "mycommon.h"

#include "SDLogger.h"
#include "SDConfigurationSingleton.h"

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


#define  PIPE_CMD_SEND           0
#define  PIPE_CMD_CLOSE          1
#define  PIPE_CMD_AUTH_OK        2



#define  TCP_CHECK_OK           1
#define  TCP_CHECK_ERROR        2
#define  TCP_CHECK_NEEDMORE     3

enum enum_notify_type
{
	//enum_sock_new_conn=0,
	enum_sock_data_recv=1,
	enum_sock_close=2,
};

//                        返回值      通知类型    数据buffer头指针  数据buffer长度
//typedef boost::function<   int     (     enum_notify_type,     char *pdata,       int )  >          type_notify_callback;

typedef boost::function<   int     (     enum_notify_type,     void *clientdata )  >          type_notify_callback;

typedef boost::function< void * (int clientfd, uint64_t tag) > type_new_connect_callback;

typedef boost::function< int (void *clientdata) > type_get_length_callback;



typedef struct _STU_EPOLL_RTN
{
	int clientfd;
	uint64_t fd_unify_tag;
	std::string socket_ientity;
	std::string str_data_send;
	int cmd_type;            //PIPE_CMD_SEND   PIPE_CMD_CLOSE
}STU_EPOLL_RTN;


typedef struct _STU_SOCKET_INFO
{
	int clientfd;
	uint64_t fd_unify_tag;
}STU_SOCKET_INFO;


class tcpserver
{
public:
	tcpserver(void);
	~tcpserver(void);

	int init_tcpserver(type_notify_callback callback, type_new_connect_callback new_connectcbk,
						type_get_length_callback getheadlengthcbk,
						type_get_length_callback getbodylengthcbk);


	int close_socket(int fd, uint64_t tag);

	int socket_auth_ok(int fd, uint64_t tag, std::string &socket_identity);
	
	int send_data(int fd, uint64_t tag, std::string &senddata);

	int send_data_ientity(std::string socket_identity, std::string &senddata);

	int get_current_conn(void);
	uint64_t get_conn_history(void);
	

private:
	int write_byte_to_pipe(void);
	int senddata_internal(std::string &data_send, int fd);
	int close_internal(int fd);
	std::map<std::string, STU_SOCKET_INFO> m_map_socket_ientity;
	block_queue<STU_EPOLL_RTN> m_queue_rtn;
	uint32_t m_sendbuf; 
	uint32_t m_sendbufsize; 
	int dealwith_new_connection(int listenfd);
	int dealwith_pipe_cmd(int pipe_read_fd);
	int dealwith_recv(int fd);
	int non_blocking(int socketfd);
	int check_timeout(void);
	void **m_array_epoll_data;
	uint64_t get_unify_tag( int client_fd );
	type_notify_callback m_callback;
	type_new_connect_callback m_callback_new_client;
	type_get_length_callback m_get_head_length_callback;
	type_get_length_callback m_get_body_length_callback;

	uint64_t m_last_timeout_check_seconds;
	int m_epoll_timeout_seconds;
	int m_check_timeout_interval_seconds;
	int m_invalid_socket_timeout_seconds;
	int m_port;
	int m_listen_fd;
	int m_epoll_maxsize;
	int m_pipe_read;
	int m_pipe_write;
	int m_pipes[2];
	int m_epoll_fd;
	int m_clientcount;
	// 曾经连接上来过的客户端总数
	uint64_t m_client_history_count;
	uint64_t m_unify_id;
	DECL_LOGGER(logger);
	static void *reactor_proc(void *param);
	void reactor_doit(void);
};
