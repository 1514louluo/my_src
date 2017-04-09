#include "verify_service.h"

#include "SDConfigurationSingleton.h"


IMPL_LOGGER(verify_service, logger);

std::vector<std::string> verify_service::m_vec_serverip;

int verify_service::m_port=0;

int verify_service::m_socket_timeout=1000;


verify_service::verify_service(void)
{
	
	
}

verify_service::~verify_service(void)
{
}

int verify_service::verify_code( std::string str_code, std::string str_codeid, std::string codetype )
{
	//codetype=SEA&codeid=3AA01F81A1657533FAE67938930DC31F&code=afsff



	std::string req = "GET /verify/" + str_code + "/" + str_codeid + "?t=" + codetype + " HTTP/1.1\r\n";
	BOOST_FOREACH(std::string str_ip, m_vec_serverip)
	{
		int fd = sopen(str_ip.c_str(), m_port, SOCK_CLIENT_TYPE);
		if (fd == -1)
		{
			MY_LOGERROR("connect verify server failed server="<<str_ip<<" port="<<m_port<<" retry next server");
			continue;
		}

		// 开始发送数据
		SDUtility::UTF8ToGBK(req);
		int data_sended = ssend_data(fd, req.c_str(), req.size(), m_socket_timeout);
		if (data_sended != req.size())
		{
			sclose(fd);
			MY_LOGERROR("send socket error");
			continue;
		}
		char recv_buffer[1000] = {0};
		int recv_count = srecv_data(fd, recv_buffer, 1000, m_socket_timeout);
		sclose(fd);
		if (recv_count == -1)
		{
			MY_LOGERROR("recv data failed, change server, try again");
			continue;
		}

		std::string str_response;
		str_response.append(recv_buffer, recv_count);
		boost::trim(str_response);
		if (str_response == "200")
		{
			MY_LOGINFO("recv verify response="<<str_response<<"  verify ok");
			return 0;
		} 
		else
		{
			MY_LOGINFO("recv verify response="<<str_response<<"  verify failed");
			return -1;
		}
		//MY_LOGINFO("recv verify response="<<str_response);
		//return (int)SDUtility::atouint32(str_response.c_str(), str_response.length());

	}

	return -1;
	

}

void verify_service::INIT_VERIFY_SERVICE( void )
{

	const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
	std::string str_verify_ips = config.getString("verifyservers", "127.0.0.1");
	std::vector<std::string> vec;
	boost::split(vec, str_verify_ips, boost::is_any_of(","));
	if (vec.size() == 0)
	{
		m_vec_serverip.push_back("127.0.0.1");
	}
	else
	{
		BOOST_FOREACH(std::string str_domain, vec)
		{
			std::string str_ip;
			SDUtility::gethubIP(str_ip,str_domain.c_str());
			m_vec_serverip.push_back(str_ip);
			MY_LOGINFO("push verify server ip="<<str_ip);
		}
	}
	m_port = config.getInt("verifyserver_port",4444);
	m_socket_timeout = config.getInt("verifyserver_timeout", 5000);


}
