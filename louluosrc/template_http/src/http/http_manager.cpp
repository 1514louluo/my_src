#include "http_manager.h"

#include "dispatcher.h"
#include "config_singleton.h"

IMPL_LOGGER(http_manager, logger)

http_manager::http_manager(void)
{
	m_pparent = NULL;
	m_requst_count = 0;
}

http_manager::~http_manager(void)
{
}

int http_manager::init_http_manager( void )
{
	const configref& config = config_singleton::get_instance()->get_config();
	int port = config.getInt("property_server_port", 2222);

	m_httpserver.init_server(port, http_manager::http_callback, 60000, (void *)this);

	return 0;

}

int http_manager::http_callback( enum_notify_type notify_type, const char *pdata, int length, 
								STU_EPOLL_DATA *pstu_epoll_data,void *context )
{
	http_manager *pthis = (http_manager *)context;
	if (notify_type == enum_sock_new_conn)
	{
		//SYS_LOG_INFO("new connection arrives, fd="<<clientfd<<"  tag="<<client_unify_tag);
		return HTTP_CHECK_DATA_OK;
	}
	if (notify_type == enum_sock_close)
	{
		//SYS_LOG_INFO("client closed, clientfd="<<clientfd<<" client_unify_tag="<<client_unify_tag);
		return HTTP_CHECK_DATA_OK;
	}

	if (pdata == NULL)
	{
		return HTTP_CHECK_DATA_OK;
	}
	
	int parse_result = pthis->parse_http_request(pdata, length, pstu_epoll_data->clientfd, 
									pstu_epoll_data->fd_unify_tag);

	
	
	//strrtn = strrtn + str_body;
	//SYS_LOG_INFO("respond strrtn="<<strrtn);

	if (parse_result == -1)
	{
		std::string str_request_log;
		str_request_log.append(pdata, length);
		SYS_LOG_ERROR("http parse request error, request string="<<str_request_log);
		return HTTP_CHECK_DATA_ERROR;
	} 
	else if (parse_result == 0)
	{
		std::string strsend;
		strsend = pdata;
		
		return HTTP_CHECK_DATA_OK;
	}
	else // parse_result == 1
	{
		return HTTP_CHECK_DATA_NEEDMORE;
	}


	return HTTP_CHECK_DATA_ERROR;

}

int http_manager::parse_http_request( const char *pdata, int length , int clientfd, uint64_t client_unify_tag)
{
	std::string strdata;
	strdata.append(pdata,length);
	
	
	boost::shared_ptr<HttpRequestPacket> prquest(new HttpRequestPacket);
	int decode_result = prquest->Decode(strdata);
	if (decode_result == -1  || decode_result == -2)
	{
		return -1;
	}

	//prquest->LOGDEBUG();
	dispatcher *pdispatcher = (dispatcher *)m_pparent;
	m_requst_count++;
	/*if (m_requst_count % 1000 == 0)
	{
		SYS_LOG_INFO("request count="<<m_requst_count<<" current cnn="<<m_httpserver.get_client_count());
	}*/
	prquest->m_client_tag = client_unify_tag;
	prquest->m_clientfd = clientfd;
	prquest->m_addqueue_time_millsec = SDUtility::get_cur_millseconds();
	prquest->generate_client_ip();
	pdispatcher->m_request_queue.add_queue(prquest);
	return 0;

}

void http_manager::set_parent( void *param )
{
	m_pparent = param;
}
