#pragma once

#include "mycommon.h"

class socket_client
{
public:
	socket_client(void);
	~socket_client(void);
	
	int m_fd;
	uint64_t m_unify_tag;
	std::string m_data_recv;  // 接收的缓冲区数据
	std::string m_data_send;  // 发送的缓冲区数据
	bool m_is_authed;         // 是否已经权限验证通过
	std::string m_socket_identity; // 对于本socket的uid标识
	uint64_t m_last_update_seconds; // 标记最后更新时间
	std::string m_clientip;
};
