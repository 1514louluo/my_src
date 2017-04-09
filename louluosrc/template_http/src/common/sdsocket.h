/******************************
 *
 *
 *****************************/

#ifndef SD_SOCKET_H_20070924
#define SD_SOCKET_H_20070924

#include <stdint.h>
#include "logdef.h"
enum SOCK_TYPE {SOCK_CLIENT_TYPE, SOCK_SERVER_TYPE};
enum SNDRCV_TYPE {SOCK_SEND_TYPE=1, SOCK_RECV_TYPE, SOCK_SNDRCV_TYPE};
typedef int sock_t;
#define SOCK_INITIALIZER (-1);

sock_t sopen(const char* ip_address, int port, SOCK_TYPE st);
int sclose(sock_t sd);

sock_t srecv_client(sock_t sd, uint32_t* client_ip, int* client_port, int timeout_ms);
int srecv_data(sock_t sd, char* recv_buf, int recv_bytes, int timeout_ms, int timeout_ms2=0);
int ssend_data(sock_t sd, const char* send_buf, int send_bytes, int timeout_ms);

int sset_bufsize(sock_t sd, uint32_t buf_size, SNDRCV_TYPE rwt);

// 读取指定长度
int timeout_recv(sock_t sd, char * recv_buf, int recv_bytes, int timeout_second);

// 只完成一次读取即返回
int timeout_recv_once(int fd, char * recv_buf, int recv_bytes, int timeout_second);
//DECL_LOGGER(logger);
#endif

