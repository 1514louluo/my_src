#ifndef HTTP_REQUEST_PACKET_H
#define HTTP_REQUEST_PACKET_H

#include <errno.h>

#include "common/SDLogger.h"
#include <string>
#include <map>
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
#include "boost/algorithm/string.hpp"
using namespace std;

struct HttpRequestPacket
{
	HttpRequestPacket();
	~HttpRequestPacket();
	int Decode(std::string& sHttpHead);
	void ParseParam(const string& sHttpRequest);
	void ParseCookie(const string& sCookie);
	void ParseHead(const string& sHttpHead);
	static int UTF8ToGBK(string& sStr);
	std::string get_request_url(void);

	void generate_client_ip(void);

	std::string get_client_ip(void);

	string m_str_GetRequest;
	string m_str_cmdpath;
	map<string, string> mapParam;
	map<string, string> mapCookie;
	map<string, string> mapHead;


	// 主要负责标记业务层的数据，严格来说，应该做分离
	int m_clientfd;
	uint64_t m_client_tag;

	// 做日志标记
	std::string str_log_username;
	std::string str_log_gameinternalno;
	std::string str_log_sessionid;
	std::string str_log_xlinternalno;

	// 请求类型
	int m_request_type;
	// 接收请求时间
	uint64_t m_addqueue_time_millsec;
	uint64_t m_dequeue_time_millsec;
	uint64_t m_start_processtime;
	uint64_t m_response_time_millsec;

	std::map<std::string, std::string> m_unixtime_tag_map;
	std::string m_costtime;

	void LOGDEBUG();

private:

	std::string m_str_clientip;
	string GetLine(const string& sWhichDomain, const string& sHttpHead);
	
	
	static int U2G(char *pszInBuf, int iInLen, char *pszOutBuf, int iOutLen);
	static int CodeConvert2(const char *pszFromCharSet, const char *pszToCharSet, char *pszInBuf, size_t iInLen,    char *pszOutBuf, size_t iOutLen); 

	static void UpperCase(string& sText);
	static void unescape_url(char *url);
	static bool Unescape_url(string &url);   
	static unsigned char x2c(const char *what);
	static string TrimLeft(string &str);
	static string TrimRight(string &str);
	static string Trim(string &str);
    int32_t checkReferer(string& referer);

	int decode_post(string& sHttpHead);
	int decode_get(string& sHttpHead);
	
private:
	DECL_LOGGER(logger);
 
};

#endif




