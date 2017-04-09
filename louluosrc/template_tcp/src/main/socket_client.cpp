#include "socket_client.h"

socket_client::socket_client(void)
{
	m_is_authed = false;
	m_last_update_seconds = time(NULL);
}
 
socket_client::~socket_client(void)
{
}
