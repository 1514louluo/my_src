#include "HttpResponseHeaderPacket.h"


IMPL_LOGGER(HttpResponseHeaderPacket, logger);

std::string HttpResponseHeaderPacket::HttpHeaderInfo(int iHttpCode, int iContentLength, const std::string& sContentType)
{           
    std::string sCodeDesc = "OK";
    switch( iHttpCode )
    {       
        case 200 :
            sCodeDesc = "OK";
            break;
        case 403:
            sCodeDesc = "Forbidden";
            break; 
        case 404:
            sCodeDesc = "Not Found";
            break;
        case 500:
            sCodeDesc = "Internal Server Error";
            break;
    }
	char pszTmp[512]={0};
    sprintf(pszTmp, "HTTP/1.1 %d %s\r\nContent-Length:%d\r\nConnection: close\r\nContent-Type:%s\r\n\r\n",
		iHttpCode, sCodeDesc.c_str(), iContentLength, sContentType.c_str());

	//sprintf(pszTmp, "HTTP/1.1 %d %s\r\nTransfer-Encoding:chunked\r\nConnection: close\r\nContent-Type:%s\r\n\r\n",
	//	iHttpCode, sCodeDesc.c_str(), sContentType.c_str());

	
	//sprintf(pszTmp, "HTTP/1.1 %d %s\r\nConnection: close\r\nContent-Type:%s\r\n\r\n",
	//	iHttpCode, sCodeDesc.c_str(), sContentType.c_str());

	//sprintf(pszTmp, "HTTP/1.1 %d %s\r\n\r\n",
	//	iHttpCode,sCodeDesc.c_str());

    return std::string(pszTmp);
}

std::string HttpResponseHeaderPacket::HttpHeaderCompressInfo(int iHttpCode, int iContentLength, const std::string& sContentType)
{           
   std:: string sCodeDesc = "OK";
    switch( iHttpCode )
    {       
        case 200 :
            sCodeDesc = "OK";
            break;
        case 403:
            sCodeDesc = "Forbidden";
            break; 
        case 404:
            sCodeDesc = "Not Found";
            break;
        case 500:
            sCodeDesc = "Internal Server Error";
            break;
    }
	char pszTmp[512]={0};
    sprintf(pszTmp, "HTTP/1.1 %d %s\r\nContent-Length:%d\r\nConnection: close\r\nContent-Type:%s;charset=utf-8\r\n\r\n", 
		iHttpCode, sCodeDesc.c_str(), iContentLength, sContentType.c_str());
    return std::string(pszTmp);
}

void HttpResponseHeaderPacket::encode( int iHttpCode, std::map<std::string, std::string> &body_map, const std::string& sContentType/*="text/html"*/ )
{
	std::string str_body;
	map2string(str_body, body_map);
	
	m_str_respond = HttpHeaderInfo(iHttpCode, str_body.size());
	m_str_respond = m_str_respond + str_body;
	//m_str_respond = m_str_respond +"\r\n\r\n";
	//MY_LOGINFO("m_str_respond="<<m_str_respond);

}

void HttpResponseHeaderPacket::map2string(std::string &str_result, std::map<std::string, std::string> &map_src)
{

	std::map<std::string, std::string>::iterator iter;
	for (iter = map_src.begin(); iter != map_src.end(); iter++)
	{
		str_result = str_result+iter->first+"="+iter->second+"&";
	}
	// 去掉最后一个& 符号
	if ( ! str_result.empty())
	{
		str_result.erase(str_result.length()-1);
	}


}

