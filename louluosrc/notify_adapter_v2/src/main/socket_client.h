#pragma once

#include "mycommon.h"

class socket_client
{
public:
	socket_client(void);
	~socket_client(void);
	
	int m_fd;
	uint64_t m_unify_tag;
	std::string m_data_recv;  // ���յĻ���������
	std::string m_data_send;  // ���͵Ļ���������
	bool m_is_authed;         // �Ƿ��Ѿ�Ȩ����֤ͨ��
	std::string m_socket_identity; // ���ڱ�socket��uid��ʶ
	uint64_t m_last_update_seconds; // ���������ʱ��
	std::string m_clientip;
};
