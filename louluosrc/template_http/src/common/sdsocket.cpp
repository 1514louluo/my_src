#include "sdsocket.h"

#include "sdtime.h"

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "logdef.h"
#include <stdio.h> // for test

//IMPL_LOGGER(sdsocket, logger);
#define SOCK_INIT (-1)
#define IS ==

sock_t sopen(const char * ip_address, int port, SOCK_TYPE st)
{
	    //SYS_LOG_ERROR("heloi");  
        sock_t sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sd IS -1) 
		{
                return sd;
        }

        bool ok = false;
        do {
                if (st IS SOCK_CLIENT_TYPE) 
				{ /* CLIENT */
                        struct sockaddr_in addr;
                        memset(&addr, 0, sizeof(addr));
                        addr.sin_family = AF_INET;
                        addr.sin_addr.s_addr = inet_addr(ip_address);
                        addr.sin_port = htons(port);

                        if (connect(sd, (const struct sockaddr*)&addr, (socklen_t)sizeof(addr)) IS -1) {
                                break;
                        }

                        int flag = -1;
                        /*if ((flag=fcntl(sd, F_GETFL)) IS -1
                                        || fcntl(sd, F_SETFL, flag|O_NONBLOCK) IS -1) {
                                break;
                        }*/

						//int nNetTimeout=7000;//7秒，
						////设置发送超时
						//int result = setsockopt(sd,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
						//if (result != 0)
						//{
						//	printf("setsockopt SEND timeout failed,errno=%d,strerr=%s\n",errno,strerror(errno));
						//}
						//// 设置接收超时
						//result = setsockopt(sd,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
						//if (result != 0)
						//{
						//	printf("setsockopt RECV timeout failed,errno=%d,strerr=%s\n",errno,strerror(errno));
						//}


                        ok = true;
                } 
				else if (st IS SOCK_SERVER_TYPE) 
				{ /* SERVER */
                        /* set reuse */
                        int reuse = 1;
                        if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) IS -1) 
						{
                                break;
                        }

                        struct sockaddr_in addr;
                        memset(&addr, 0, sizeof(addr));
                        addr.sin_family = AF_INET;
                        addr.sin_addr.s_addr = inet_addr(ip_address);
                        addr.sin_port = htons(port);

                        if (bind(sd, (const struct sockaddr*)&addr, (socklen_t)sizeof(addr)) IS -1) 
						{
                                break;
                        }

                        if (listen(sd, SOMAXCONN) IS -1) 
						{
                                break;
                        }

                        ok = true;
                }
        } 
		while (false);

        if (!ok) 
		{
                sclose(sd);
                sd = SOCK_INIT;
        }

        return sd;
}

int sclose(sock_t sd)
{
        return close(sd);
}

sock_t srecv_client(sock_t sd, uint32_t * client_ip, int * client_port, int timeout_ms)
{
        struct pollfd poll_fd;
        memset(&poll_fd, 0, sizeof(poll_fd));

        poll_fd.fd = sd;
        poll_fd.events = (POLLIN | POLLRDNORM);
        poll_fd.revents = 0;
        int poll_ret = poll(&poll_fd, 1, timeout_ms);
        if (poll_ret IS -1) {
                return -1;
        } else if (poll_ret IS 0) {
                return 0;
        }

        if (poll_fd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
                return -2;
        }

        struct sockaddr_in new_addr;
        memset(&new_addr, 0, sizeof(new_addr));
        socklen_t new_addr_len = sizeof(new_addr);

        sock_t new_sd = accept(sd, (struct sockaddr*)&new_addr, &new_addr_len);
        if (new_sd IS -1) {
                return new_sd;
        }

        int flag = -1;
        if ((flag=fcntl(new_sd, F_GETFL)) IS -1
                        || fcntl(new_sd, F_SETFL, flag|O_NONBLOCK) IS -1) {
                sclose(new_sd);
                new_sd = SOCK_INIT;
                return new_sd;
        }

        if (client_ip) {
                *client_ip = new_addr.sin_addr.s_addr;
        }
        if (client_port) {
                *client_port = new_addr.sin_port;
        }

        return new_sd;
}


int timeout_recv_once(int fd, char * recv_buf, int recv_bytes, int timeout_second)
{
	if (recv_buf == NULL)
	{
		return -1;
	}

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	int result;
	timeval tv;
	tv.tv_sec = timeout_second;
	tv.tv_usec = 0;
	result = select(fd+1, &fds, NULL, NULL,&tv);
	if (result == -1)
	{
		SOCK_ERR_LOG("select return -1, errno="<<errno<<" errstr="<<strerror(errno));
		return -1;
	}
	if (result == 0)
	{
		//printf("select timeout %d seconds\n", timeout_second);
		SOCK_ERR_LOG("select timeout "<<timeout_second<<" seconds");
		return -1;
	}
	result = FD_ISSET(fd, &fds);
	if (result == false)
	{
		SOCK_ERR_LOG("FD_ISSET(fd, &fds)  == false error");
		return -1;
	}

	// 开始读取数据
	//SOCK_ERR_LOG("ready to read");
	result = recv(fd, recv_buf, recv_bytes, 0);
	//SOCK_ERR_LOG("read finished");

	return result;





}

// 可能返回0或已经读到的数，必须读取到指定字节长度的数据或超时才会返回
int timeout_recv(sock_t sd, char * recv_buf, int recv_bytes, int timeout_second)
{
	if (recv_buf == NULL)
	{
		return -1;
	}
	
	time_t time_start = time(NULL);
	time_t time_end = time_start+ timeout_second;
	time_t currenttime=0;
	int bytes_read_total=0;
	while (currenttime=time(NULL) <= time_end)
	{
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(sd, &fds);
		int result;
		timeval tv;
		tv.tv_sec = time_end-time(NULL);
		tv.tv_usec = 0;
		if (tv.tv_sec == 0)
		{
			SOCK_ERR_LOG("tv.tv_sec == 0 timeout error");
			return -1;
		}
		//SOCK_ERR_LOG("ready to select");
		result = select(sd+1, &fds, NULL, NULL,&tv);
		//SOCK_ERR_LOG("select finished, result="<<result);
		if (result == -1)
		{
			return -1;
		}
		if (result == 0)
		{
			//printf("select timeout %d seconds\n", timeout_second);
			//SOCK_ERR_LOG("select timeout "<<timeout_second<<" seconds");
			//return -1;
			continue;
		}
		result = FD_ISSET(sd, &fds);
		if (result == false)
		{
			SOCK_ERR_LOG("FD_ISSET(sd, &fds)  == false error");
			return -1;
		}

		int read_bytes_once;
		// 开始读取数据
		//SOCK_ERR_LOG("ready to read");
		read_bytes_once = recv(sd, recv_buf+bytes_read_total, recv_bytes-bytes_read_total, 0);
		//SOCK_ERR_LOG("read finished");
		if (read_bytes_once == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) 
			{
				continue;
			}
			SOCK_ERR_LOG("recv error, errno="<<errno<<" errstr="<<strerror(errno));
			return -1;
		}
		if (read_bytes_once == 0)
		{
			SOCK_ERR_LOG("socket closed by peer");
			return bytes_read_total;
		}
		bytes_read_total = bytes_read_total+read_bytes_once;
		if (bytes_read_total == recv_bytes)
		{
			return bytes_read_total;
		}


	}
	SOCK_ERR_LOG("recv time out error");
	return -1;
	

}

int srecv_data(sock_t sd, char * recv_buf, int recv_bytes, int timeout_ms, int timeout_ms2)
{
		int againcount=0;
		int pollcount=0;
        struct pollfd poll_fd;
        memset(&poll_fd, 0, sizeof(poll_fd));
        poll_fd.fd = sd;

        int total_recv_bytes = 0;
        int left_bytes = recv_bytes;
        int left_time_ms = timeout_ms;
        uint32_t calc_time_start = get_cur_millseconds();
        do {
                int recv_ret = recv(sd, recv_buf+total_recv_bytes, left_bytes, 0);
                if (recv_ret > 0) 
				{
                        total_recv_bytes += recv_ret;
                        left_bytes -= recv_ret;
                } 
				else if (recv_ret IS 0) 
				{
                        return total_recv_bytes;
                }
				else 
				{
					//printf("recv rtn=-1, errno=%d, strerrno=%s\n", errno, strerror(errno));
                        if (errno IS EAGAIN || errno IS EWOULDBLOCK || errno IS EINTR) 
						{
							/*againcount++;
							if (againcount < 3)
							{
								continue;
							}
							else 
							{
								return recv_ret;
							}*/
                                
                        } 
						else 
						{
                                return recv_ret;
                        }
                }

                if (left_bytes <= 0 || left_time_ms IS 0) 
				{
                        break;
                }

                poll_fd.events = (POLLIN | POLLRDNORM);
                poll_fd.revents = 0;
                int poll_ret = poll(&poll_fd, 1, left_time_ms);
                if (poll_ret IS -1) 
				{
					if (errno IS EINTR)
					{
						//printf("errno IS EINTR pollcount=%d\n", pollcount);
						pollcount++;
						if (pollcount < 3000)
						{
							continue;
						} 
						else
						{
							return recv_ret;
						}
					} 
					else
					{
						 return -1;
					}
                       
                } 
				else if (poll_ret IS 0) 
				{
						//printf("poll_ret IS 0 total_recv_bytes=%d\n", total_recv_bytes);
                        return total_recv_bytes;
                }
#if 0
                if (poll_fd.revents & (POLLERR | POLLHUP | POLLNVAL)) 
				{
                        return -2;
                }
#endif
                uint32_t calc_time_stop = get_cur_millseconds();
                if (left_time_ms < 0) 
				{
                        if (timeout_ms2 > 0) 
						{
                                left_time_ms = timeout_ms2;
                                calc_time_start = calc_time_stop;
                        } 
						else 
						{
                                left_bytes = 0;
                        }
                } else if (left_time_ms == 0) 
				{
                        left_bytes = 0;
                } 
				else if ((calc_time_stop - calc_time_start)>=(uint32_t)timeout_ms) 
				{
                        left_bytes = 0;
                } 
				else 
				{
                        left_time_ms = timeout_ms - (calc_time_stop - calc_time_start);
                }
        } while (left_bytes > 0);

        return total_recv_bytes;

}

int ssend_data(sock_t sd, const char * send_buf, int send_bytes, int timeout_ms)
{
        struct pollfd poll_fd;
        memset(&poll_fd, 0, sizeof(poll_fd));
        poll_fd.fd = sd;	

        int total_send_bytes = 0;
        int left_bytes = send_bytes;
        int left_time_ms = (timeout_ms<=0) ? 0 : timeout_ms;
        const uint32_t calc_time_start = get_cur_millseconds();
        do
		{
				//printf("ready to send data\n");
                int send_ret = send(sd, send_buf+total_send_bytes, left_bytes, 0);
				//printf("send data finished\n");
                if (send_ret >= 0) 
				{
                        total_send_bytes += send_ret;
                        left_bytes -= send_ret;
                } else 
				{
					//printf("send data rtn -1, errno=%d, errstr=%s\n", errno, strerror(errno));
                        if (errno IS EAGAIN || errno IS EWOULDBLOCK || errno IS EINTR) {
                                // continue
                        } 
						else 
						{
                                return send_ret;
                        }
                }

                if (left_bytes <= 0 || left_time_ms IS 0) 
				{
                        break;
                }

                poll_fd.events = (POLLOUT | POLLWRNORM);
                poll_fd.revents = 0;
                int poll_ret = poll(&poll_fd, 1, left_time_ms);
                if (poll_ret IS -1) 
				{
                        return -1;
                } 
				else if (poll_ret IS 0) 
				{
                        return total_send_bytes;
                }
#if 0
                if (poll_fd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
                        return -2;
                }
#endif
                uint32_t calc_time_stop = get_cur_millseconds();
                if ((calc_time_stop - calc_time_start)>=(uint32_t)timeout_ms) 
				{
                        left_bytes = 0;
                } 
				else 
				{
                        left_time_ms = timeout_ms - (calc_time_stop - calc_time_start);
                }
        }            
		while (left_bytes > 0);

        return total_send_bytes;
}

int sset_bufsize(sock_t sd, uint32_t buf_size, SNDRCV_TYPE rwt)
{
        int ret_val = -1;

        if (rwt | SOCK_SEND_TYPE) {
                ret_val = setsockopt(sd, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size));
                if (ret_val != 0) {
                        return ret_val;
                }
        }

        if (rwt | SOCK_RECV_TYPE) {
                ret_val = setsockopt(sd, SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof(buf_size));
        }

        return ret_val;
}


