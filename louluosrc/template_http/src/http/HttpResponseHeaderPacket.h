#ifndef HTTP_RESPONSE_HEADER_PACKET_H
#define HTTP_RESPONSE_HEADER_PACKET_H

#include <errno.h>

#include "logdef.h"
//#include <common/SDClientSocket.h>
//#include "SDDefine.h"
#include <string>
#include <map>
//#include "common/md5.h"
#include "mycommon.h"
#define  HTTP_RESPONSE_STATUS_OK		     200
#define  HTTP_RESPONSE_STATUS_FORBIDDEN      403
#define  HTTP_RESPONSE_STATUS_NOTFOUND       404
#define  HTTP_RESPONSE_STATUS_INTERNALERR    500




class HttpResponseHeaderPacket
{
public:

	void encode(int iHttpCode, std::map<std::string, std::string> &body_map, const std::string& sContentType="text/html");
	std::string HttpHeaderInfo(int iHttpCode, int iContentLength=0, const std::string& sContentType="text/html"); 
	std::string HttpHeaderCompressInfo(int iHttpCode, int iContentLength=0, const std::string& sContentType="text/html");
	//static int encode_response(SDClientSocket* socket_ptr, std::string& res);
	//static int encode_response(SDClientSocket* socket_ptr, char* buf, int size);
	//static std::string get_xbase_key(int flag, HttpRequestPacket* pRequest);
	//static string get_content_type(const int& cmd_id);
	//static std::string md5_sum(std::string &xdriverid, std::string &uid);

	std::string m_str_respond;
	DECL_LOGGER(logger);

	

private:
	
	void map2string(std::string &str_result, std::map<std::string, std::string> &map_src);
};

#endif

