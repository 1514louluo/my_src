#pragma once

#include "tcpserver.h"
#include "socket_client.h"
#include "SDLogger.h"
#include "message_codec.h"
#include "protocol_codec.h"
#include "xltimer.h"

enum enum_dispatcher_reply
{
	enum_dispatcher_close_socket=0,
	enum_dispatcher_auth_ok=1,
	enum_dispatcher_senddata=2,
};

typedef struct _STU_DISPATCHER_REPLY
{
	enum_dispatcher_reply enum_type;
	std::string gameinternalno;
	int fd;
	uint64_t tag;
	std::string senddata;
}STU_DISPATCHER_REPLY;

class tcpservermanager
{
public:
	tcpservermanager(void);
	~tcpservermanager(void);

	int init_tcpserver_manager(void);
	void setparent(void *param);

	int reply_data(int fd, uint64_t tag, std::string &strdata);
	int reply_data_identity(std::string sock_ientity, std::string &strdata);
	int reply_close(int fd, uint64_t tag);
	int reply_close_identity(std::string sock_identity);
	int reply_auth_ok(int fd, uint64_t tag,  std::string sock_identity);
	int boadcast(std::string strdata);


private:
	DECL_LOGGER(logger);
	tcpserver m_tcpserver;
	xltimer m_timer_get_stat;
	void *m_parent;

	int data_notify_callback(enum_notify_type notify_type, void *pclientdata);
	void * new_connect_callback(int newfd, uint64_t unify_tag);
	int get_head_length(void *pclientdata);
	int get_body_length(void *pclientdata);
	
	void get_stat_timer_callback(void);
	

	//block_queue<STU_DISPATCHER_REPLY> m_queue_dispatcher_reply;
	//static void *dispatcher_reply_proc(void *param);
	//void dispatcher_reply_doit(void);

	//typedef boost::function<   int     (     enum_notify_type,     void *clientdata )  >          type_notify_callback;

	//typedef boost::function< void * (int clientfd, uint64_t tag) > type_new_connect_callback;



};
