#include "HttpRequestPacket.h"
#include "config_singleton.h"
#include "SDUtility.h"
#include "mycommon.h"
//#include "myserver.h"
IMPL_LOGGER(HttpRequestPacket, logger);

HttpRequestPacket::HttpRequestPacket()
{
	m_client_tag = 0;
	m_clientfd = 0;

}

HttpRequestPacket::~HttpRequestPacket()
{
	//SYS_LOG_DEBUG("delete HttpRequestPacket");
}


int HttpRequestPacket::Decode(std::string& sHttpHead)
{
	//SYS_LOG_DEBUG(sHttpHead);
	string::size_type requestPos = sHttpHead.find("\r\n");
	if ( requestPos == string::npos )
	{   // unfinished recv data, need more
		SYS_LOG_DEBUG("error http request : " << sHttpHead);
		return -2;
	}

	//ready to parse param
	string sRequestLine = sHttpHead.substr(0, requestPos);
	if (sRequestLine.find("GET") != std::string::npos)
	{
		return decode_get(sHttpHead);
	}
	if (sRequestLine.find("POST") != std::string::npos)
	{
		return decode_post(sHttpHead);
	}
	return -1;



/*
    //read host
    pos2 = sHttpHead.find("Host:");
    if ( pos2 == string::npos )
    {
        SYS_LOG_DEBUG("can't find host ");
        return -1;
    }
    else
    {
      const configref& config = config_singleton::get_instance()->get_config();
	  
      string  host = config.getString("host", "vod");
      pos2 = sHttpHead.find(host,pos2);
         if ( pos2 == string::npos )
          {
               SYS_LOG_DEBUG( "host error ");

               return -1;
           }   
        string::size_type endPos = sHttpHead.find("\r\n",pos2);
        mapParam["host"] = sHttpHead.substr(pos2, endPos-pos2);
    }

    pos2 = sHttpHead.find("Referer:");
    if( pos2 == string::npos )
    {
        SYS_LOG_DEBUG( "can't find REferer "); 
        return -1;
    }
    else
    {
        string::size_type endPos = sHttpHead.find("\r\n",pos2);
        string referer = sHttpHead.substr(pos2, endPos-pos2);
        return checkReferer(referer);
    }
*/
	return 0;
}

void HttpRequestPacket::ParseHead(const string& sHttpHead)
{
	string::size_type lineBegin = 0;
	string::size_type lineEnd = sHttpHead.find("\r\n");
	while( lineEnd != string::npos )	
	{
		//get line
		string sLine = sHttpHead.substr(lineBegin, lineEnd-lineBegin); 
		string::size_type diffPos = sLine.find(":");
		if ( diffPos == string::npos )
		{
			lineBegin = lineEnd+2;
			lineEnd = sHttpHead.find("\r\n", lineBegin);
			continue;	
		}
		string sDomain = sLine.substr(0, diffPos);	
		string sValue  = sLine.substr(diffPos+1);
		HttpRequestPacket::Trim(sDomain);
		HttpRequestPacket::UpperCase(sDomain);
		mapHead[sDomain] = sValue;
		lineBegin = lineEnd+2;
		lineEnd = sHttpHead.find("\r\n", lineBegin);
	}
}

void HttpRequestPacket::ParseCookie(const string& sCookie)
{
	if ( sCookie.empty() )
		return;
	string sQuery = sCookie;
	//first delete all space charset
	sQuery.erase(remove(sQuery.begin(), sQuery.end(), ' '), sQuery.end());

	string::size_type paramStartPos = 0;
	string::size_type paramEndPos;
	string sParam;
	string::size_type valuePos;

	while ( (paramEndPos = sQuery.find(";", paramStartPos)) != string::npos )
	{
		sParam = sQuery.substr(paramStartPos, paramEndPos-paramStartPos);
		paramStartPos = paramEndPos+1;
		if ( (valuePos = sParam.find("=")) == string::npos)
			continue;//not a right name-value pair
		string sValue = sParam.substr(valuePos+1);
		HttpRequestPacket::Unescape_url(sValue);
		mapCookie[sParam.substr(0, valuePos)] = sValue;
	}

	//try to find the last name-value
	sParam = sQuery.substr(paramStartPos);
	if ( (valuePos = sParam.find("=")) != string::npos)
	{
		string sValue = sParam.substr(valuePos+1);
		HttpRequestPacket::Unescape_url(sValue);
		mapCookie[sParam.substr(0, valuePos)] = sValue;
	}	
}

void HttpRequestPacket::ParseParam(const string& sHttpRequest)
{
	string::size_type paramStartPos;
	string::size_type paramEndPos;
	string sParam;
	string::size_type valuePos;

	mapParam.clear();
	string::size_type cmdPos = sHttpRequest.find("?");
	if ( cmdPos == string::npos )
	{//has not command
		m_str_cmdpath = sHttpRequest;
		//sCmd = "";
		paramStartPos = 0;
		return;
	}
	else
	{
		m_str_cmdpath = sHttpRequest.substr(0, cmdPos);//get command
		HttpRequestPacket::Unescape_url(m_str_cmdpath);
		HttpRequestPacket::UTF8ToGBK(m_str_cmdpath);
		paramStartPos = cmdPos + 1;
	}

	while ( (paramEndPos = sHttpRequest.find("&", paramStartPos)) != string::npos )
	{
		sParam = sHttpRequest.substr(paramStartPos, paramEndPos-paramStartPos);
		paramStartPos = paramEndPos+1;
		if ( (valuePos = sParam.find("=")) == string::npos)
			continue;
		string sValue = sParam.substr(valuePos+1);
		HttpRequestPacket::Unescape_url(sValue); 
		mapParam[sParam.substr(0, valuePos)] = sValue;
	}

	//find the last pair
	sParam = sHttpRequest.substr(paramStartPos);
	if ( (valuePos = sParam.find("=")) != string::npos)
	{
		string sValue = sParam.substr(valuePos+1);
		HttpRequestPacket::Unescape_url(sValue); 
		mapParam[sParam.substr(0, valuePos)] = sValue;
		//mapParam[sParam.substr(0, valuePos)] = sParam.substr(valuePos+1);    
	}
}


string HttpRequestPacket::GetLine(const string& sWhichDomain, const string& sHttpHead)
{
	string sDomain = sWhichDomain;
	HttpRequestPacket::UpperCase(sDomain);
	map<string, string>::iterator iter = mapHead.find(sDomain);
	if ( iter == mapHead.end() )
		return "";
	return iter->second;
	
	/*
	string sDomain = sWhichDomain;
	HttpRequestPacket::UpperCase(sDomain);
	string sHead = sHttpHead;
	HttpRequestPacket::UpperCase(sHead);

	string::size_type pos = sHead.find(sDomain);
	if ( pos == string::npos )
	{
		return "";
	}

	string sOut;	
	string::size_type lineEndPos = sHttpHead.find("\r\n", pos);
	string sLine = sHttpHead.substr(pos, lineEndPos-pos);
	string::size_type contentPos = sLine.find(":");
	if ( contentPos != string::npos )
	{
		sOut = sLine.substr(contentPos+1);
	}
	return sOut;
	*/
}

void HttpRequestPacket::UpperCase(string& sText)
{
	for(unsigned int i=0; i<sText.length(); i++)
		sText[i] = toupper(sText[i]);
}

/* x2c() and unescape_url() stolen from NCSA code */
void HttpRequestPacket::unescape_url(char *url)
{
    register int x,y;

    for (x=0,y=0; url[y]; ++x,++y) {
        if((url[x] = url[y]) == '%') {
            url[x] = x2c(&url[y+1]);
            y+=2;
        }
        else if ( url[y] == '+' )
        {
            url[x] = ' ';//space charset
        }
    }
    url[x] = '\0';
}

bool HttpRequestPacket::Unescape_url(string &url)
{
    char * pszUrl = strdup(url.c_str());
    unescape_url(pszUrl);
    url = pszUrl;
    free(pszUrl);

    return true;
}

unsigned char HttpRequestPacket::x2c(const char *what)
{       // stolen from cgi-html
    register char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
    return(digit);
}

void HttpRequestPacket::LOGDEBUG()
{
	SYS_LOG_INFO( "HttpRequestPacket sCmd : " << m_str_cmdpath);
	map<string, string>::iterator iter = mapParam.begin();
	for ( ; iter != mapParam.end(); ++iter)	
	{
		SYS_LOG_INFO( "HttpRequestPacket param : " << iter->first << " : " << iter->second);
	}
	for ( iter=mapCookie.begin(); iter != mapCookie.end(); ++iter)	
	{
		SYS_LOG_INFO("HttpRequestPacket cookie : " << iter->first << " : " << iter->second);
	}

	for (iter = mapHead.begin(); iter != mapHead.end(); iter++)
	{
		SYS_LOG_INFO( "HttpRequestPacket head : " << iter->first << " : " << iter->second);
	}

}

string HttpRequestPacket::TrimLeft(string &str)
{
	int pos=0;
	while( str[pos]==' ' || str[pos]=='\t' ) { pos++; }
	return str=str.substr(pos);
}

string HttpRequestPacket::TrimRight(string &str)
{
	int pos=str.length()-1;
	while( str[pos]==' ' || str[pos]=='\t' ) { pos--; }
	return str=str.substr(0, pos+1); 
}

string HttpRequestPacket::Trim(string &str)
{
	TrimLeft(str);
	return TrimRight(str);
}

int HttpRequestPacket::UTF8ToGBK(string &sStr)
{
	char* pInBuf = new char[sStr.size()+1];
	char* pOutBuf = new char[3*sStr.size()+1];

	snprintf(pInBuf, sStr.size()+1, "%s", sStr.c_str());
	HttpRequestPacket::U2G(pInBuf, sStr.size()+1, pOutBuf, 3*sStr.size()+1);
	sStr = pOutBuf;
	
	delete [] pInBuf;
	delete [] pOutBuf;

	return 1;
}

int HttpRequestPacket::U2G(char *pszInBuf, int iInLen, char *pszOutBuf, int iOutLen)
{
	return HttpRequestPacket::CodeConvert2("UTF-8", "GBK", pszInBuf, iInLen, pszOutBuf, iOutLen);
}

int HttpRequestPacket::CodeConvert2(const char *pszFromCharSet, const char *pszToCharSet, char *pszInBuf, size_t iInLen,	char *pszOutBuf, size_t iOutLen)
{
    iconv_t iConv;
    char **ppin = &pszInBuf;
    char **ppout = &pszOutBuf;

    iConv = iconv_open(pszToCharSet, pszFromCharSet);
    if(iConv < 0)
        return -1;

    memset(pszOutBuf, 0, iOutLen);
    while( iconv(iConv, ppin, &iInLen, ppout, &iOutLen) == (size_t)-1 ) {
		
	if( errno==E2BIG ) {
			break;
		}

		//skip the invalid mutibyte character
		(*ppin)++;
		iInLen--;
		
    }
   /* if(iconv(iConv, pin, &iInLen, pout, &iOutLen) <= 0)
    {
    	iconv_close(iConv);
        return -1;
    }*/

    iconv_close(iConv);
    return 0;
}

int32_t HttpRequestPacket::checkReferer(string& referer)
{
   const configref& config = config_singleton::get_instance()->get_config();
   
//   int32_t  hostNo = CMyserver::GetInstance()->GetBaseConf().GetInt("REFERER", "hostNo", 0);
   int32_t hostNo = config.getInt("hostNo", 0);
   if(hostNo == 0)
   {
       return 0;
   }
   char szHost[128];
   string::size_type pos;
   for(int32_t i=0; i<hostNo; i++)
   {
        sprintf(szHost, "referer_host_%d", i);
        pos = referer.find(config.getString(szHost, ""));
        if(pos != string::npos)
        {
            return 0;
        }
   }
   return 1;
}

std::string HttpRequestPacket::get_request_url( void )
{

	return m_str_GetRequest;
}

void HttpRequestPacket::generate_client_ip( void )
{
	
	//LOGDEBUG();
	std::map<std::string, std::string>::iterator iter;
	iter = mapHead.find("X-REAL-IP");
	if (iter != mapHead.end())
	{
		//SYS_LOG_DEBUG("set clientip="<<iter->second<<"  from header X-Real-IP");
		m_str_clientip = iter->second;
		boost::trim(m_str_clientip);
		return;
	}
	if (m_str_clientip.empty())
	{
		char peerip[18]={0};
		struct sockaddr_in peeraddr;
		memset(&peeraddr, 0, sizeof(sockaddr_in));
		socklen_t len=sizeof(sockaddr_in);
		int ret = getpeername(m_clientfd, (struct sockaddr *)&peeraddr, &len);
		if (ret != 0)
		{
			m_str_clientip = "127.0.0.1";
			SYS_LOG_ERROR("fail to get client ip from fd");
			return;
		}
		const char *ret1 = inet_ntop(AF_INET, (void *)&peeraddr.sin_addr, (char *)peerip, sizeof(peerip));
		if (ret1 == NULL)
		{
			m_str_clientip = "127.0.0.1";
			SYS_LOG_ERROR("fail to get client ip from fd");
			return;
		}
		m_str_clientip = peerip;
		SYS_LOG_DEBUG("clientip from fd="<<m_str_clientip);
	}

}

std::string HttpRequestPacket::get_client_ip( void )
{
	return m_str_clientip;
}

int HttpRequestPacket::decode_post( string& sHttpHead )
{
	string::size_type requestPos = sHttpHead.find("\r\n");
	if ( requestPos == string::npos )
	{   // unfinished recv data, need more
		SYS_LOG_DEBUG( "error http request : " << sHttpHead);
		return -1;
	}
    
	//ready to parse param
	string sRequestLine = sHttpHead.substr(0, requestPos);
	string::size_type pos1 = sRequestLine.find("/");
	if ( pos1 == string::npos )
	{
			SYS_LOG_DEBUG( "can't find /");
			return -1;
	}
	string::size_type pos2 = sRequestLine.find(" ", pos1);//find space
	if ( pos2 == string::npos )
	{
			SYS_LOG_DEBUG( "can't find space");
			return -1;
	}
	m_str_GetRequest = sRequestLine.substr(pos1, pos2-pos1);
	ParseParam(m_str_GetRequest);
	//SYS_LOG_DEBUG( "m_str_GetRequest: " << m_str_GetRequest);
	
	ParseHead(sHttpHead);

	std::map<std::string, std::string>::iterator iter;
	iter = mapHead.find("CONTENT-LENGTH");
	if (iter == mapHead.end())
	{
		SYS_LOG_DEBUG("failed to find CONTENT-LENGTH from headers");
		return -1;
	}

	std::string str_length = iter->second;
	boost::trim(str_length);
	int intlength = SDUtility::atouint32(str_length);
	if (sHttpHead.size() < intlength)
	{
		return -1;
	}
	std::string str_params;
	str_params = sHttpHead.substr(sHttpHead.length()-intlength, intlength);
	string2map(str_params, mapParam);
	return 0;
	
	 
	

	/*std::map<std::string, std::string>::iterator iter;
	for (iter = mapHead.begin(); iter!= mapHead.end(); iter++)
	{
		SYS_LOG_DEBUG( "key="<<iter->first<<"  value="<<iter->second);
	}*/

	
	//ready to parse cookie	
	//string sCookie = GetLine("COOKIE", sHttpHead);
	//ParseCookie(sCookie);


}

int HttpRequestPacket::decode_get( string& sHttpHead )
{

		//SYS_LOG_DEBUG( "http head: " << sHttpHead);
	//get first line, GET /xx?xxx
	string::size_type requestPos = sHttpHead.find("\r\n");
	if ( requestPos == string::npos )
	{   // unfinished recv data, need more
		SYS_LOG_DEBUG( "error http request : " << sHttpHead);
		return -1;
	}
    
	//ready to parse param
	string sRequestLine = sHttpHead.substr(0, requestPos);
	string::size_type pos1 = sRequestLine.find("/");
	if ( pos1 == string::npos )
	{
			SYS_LOG_DEBUG( "can't find /");
			return -1;
	}
	string::size_type pos2 = sRequestLine.find(" ", pos1);//find space
	if ( pos2 == string::npos )
	{
			SYS_LOG_DEBUG("can't find space");
			return -1;
	}
	m_str_GetRequest = sRequestLine.substr(pos1, pos2-pos1);
	ParseParam(m_str_GetRequest);
	//SYS_LOG_DEBUG("m_str_GetRequest: " << m_str_GetRequest);
	
	ParseHead(sHttpHead);

	/*std::map<std::string, std::string>::iterator iter;
	for (iter = mapHead.begin(); iter!= mapHead.end(); iter++)
	{
		SYS_LOG_DEBUG("key="<<iter->first<<"  value="<<iter->second);
	}*/

	
	//ready to parse cookie	
	string sCookie = GetLine("COOKIE", sHttpHead);
	ParseCookie(sCookie);

	return 0;
}




