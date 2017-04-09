#include "SDUtility.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdlib.h>

using namespace std;

IMPL_LOGGER(SDUtility, logger);


string SDUtility::str2hex(const char * buf,int size)
{
	string ret_str;

	char tmpbuf[10] = {0};
	const char * buffer = buf;
	int buffer_size = size;
	for (int i = 0 ;i < buffer_size; i++) {
		sprintf(tmpbuf,"%02X",(unsigned char)buffer[i]);
		ret_str += tmpbuf;
	}

	return ret_str;
}

string SDUtility::format(int i)
{
	char buff[100]={0};
	sprintf(buff, "%d", i);
	return string(buff);
}


string SDUtility::format(uint32_t i)
{
	char buff[100]={0};
	sprintf(buff, "%u", i);
	return string(buff);
}



string SDUtility::format(uint64_t i)
{
	char buff[200]={0};
	sprintf(buff, "%u", i);
	return string(buff);
}

std::string SDUtility::ip_search(void)  
{  
	int sfd, intr;  
	struct ifreq buf[16];  
	struct ifconf ifc;  
	sfd = socket (AF_INET, SOCK_DGRAM, 0);   
	if (sfd < 0)  
	{
		return "";
	}
	ifc.ifc_len = sizeof(buf);  
	ifc.ifc_buf = (caddr_t)buf;  
	if (ioctl(sfd, SIOCGIFCONF, (char *)&ifc))  
	{
		return "";
	}
	intr = ifc.ifc_len / sizeof(struct ifreq);  
	while (intr-- > 0 && ioctl(sfd, SIOCGIFADDR, (char *)&buf[intr]));  
	close(sfd);  
	char *charip = inet_ntoa(((struct sockaddr_in*)(&buf[intr].ifr_addr))-> sin_addr); 
	std::string str_ip;
	str_ip = charip;
	return str_ip;
	//return inet_ntoa(((struct sockaddr_in*)(&buf[intr].ifr_addr))-> sin_addr);  
}  


string SDUtility::format_time(time_t unix_time)
{
//	struct timeval tv;
	struct timezone tz;

	gettimeofday(NULL, &tz);

	time_t local_time = (time_t)(unix_time - tz.tz_minuteswest * 60);

	struct tm lt;
	gmtime_r(&local_time, &lt);

	char timebuf[64] = {0};
	sprintf(timebuf, "%4d-%02d-%02d %02d:%02d:%02d", (1900+lt.tm_year), lt.tm_mon+1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);

	return (string)timebuf;
}

std::string SDUtility::get_current_time()
{
	time_t timep;
    struct tm *ptr_tm;

    time(&timep);
    ptr_tm = localtime(&timep);
	
	char timebuf[64] = {0};
	sprintf(timebuf, "%4d-%02d-%02d %02d:%02d:%02d", (1900+ptr_tm->tm_year), ptr_tm->tm_mon+1, ptr_tm->tm_mday, ptr_tm->tm_hour, ptr_tm->tm_min, ptr_tm->tm_sec);

	return (string)timebuf;
}

string SDUtility::format_day(time_t unix_time)
{
//	struct timeval tv;
	struct timezone tz;

	gettimeofday(NULL, &tz);

	time_t local_time = (time_t)(unix_time - tz.tz_minuteswest * 60);

	struct tm lt;
	gmtime_r(&local_time, &lt);

	char timebuf[16]={0};
	sprintf(timebuf, "%04d%02d%02d", lt.tm_year + 1900,lt.tm_mon+1,lt.tm_mday);

	return (string)timebuf;
}

string SDUtility::format_day_fake(time_t unix_time)
{
	//	struct timeval tv;
	struct timezone tz;

	gettimeofday(NULL, &tz);

	time_t local_time = (time_t)(unix_time - tz.tz_minuteswest * 60);

	struct tm lt;
	gmtime_r(&local_time, &lt);

	char timebuf[16]={0};
	sprintf(timebuf, "%04d%02d%02d", lt.tm_year + 2900,lt.tm_mon+1,lt.tm_mday);

	return (string)timebuf;
}

uint32_t  SDUtility::get_remind_secs_from_now2tomorrow(void)
{
	time_t now = time(NULL);
	struct tm *tm_now = localtime(&now);
	tm_now->tm_hour = 23;
	tm_now->tm_min = 59;
	tm_now->tm_sec = 59;
	time_t tomorrow = mktime(tm_now) + 1;
	uint32_t diff = (uint32_t)difftime(tomorrow, now);
	return diff;
}
string SDUtility::get_local_datestring()
{
/*	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	time_t local_time = (time_t)(tv.tv_sec - tz.tz_minuteswest * 60);
*/
	time_t local_time = time(NULL);
	struct tm lt;
//	gmtime_r(&local_time, &lt);
	localtime_r(&local_time, &lt);

	char timebuf[32]={0};
	sprintf(timebuf, "%04d%02d%02d", (lt.tm_year + 1900), (lt.tm_mon + 1), lt.tm_mday);

	return (string)timebuf;
}


string SDUtility::format_ip(uint32_t ip)
{
		struct in_addr inaddr;
		inaddr.s_addr = ip;
		return inet_ntoa(inaddr); 
}

uint8_t SDUtility::atouint8(char* ptr, int32_t len)
{
	if (len== 0)
	{
		return 0;
	}
	uint8_t value = 0;
	while((len >= 0) && isdigit(*ptr )){
		value = 10 * value + (uint32_t)(*ptr - '0');
		ptr ++;
		len --;
	}
	return value;
}

uint32_t SDUtility::atouint32(const char* ptr, int32_t len)
{
	if (len == 0)
	{
		return 0;
	}
	uint32_t value = 0;
	while((len >= 0) && isdigit(*ptr )){
		value = 10 * value + (uint32_t)(*ptr - '0');
		ptr ++;
		len --;
	}
	return value;
}
			
uint64_t SDUtility::atouint64(char* ptr, int32_t len)
{
	if (len==0)
	{
		return 0;
	}
	uint64_t value = 0;
	while((len >= 0) && isdigit(*ptr )){
		value = 10 * value + (uint32_t)(*ptr - '0');
		ptr ++;
		len --;
	}
	return value;
}

uint32_t SDUtility::atouint32(std::string &str)
{
	return atouint32(str.c_str(), str.size());
}

uint64_t SDUtility::atouint64(std::string &str)
{
	return atouint64(str.c_str(), str.size());
}



uint64_t SDUtility::atouint64(const char* ptr, int32_t len)
{
	uint64_t value = 0;
	while((len >= 0) && isdigit(*ptr )){
		value = 10 * value + (uint32_t)(*ptr - '0');
		ptr ++;
		len --;
	}
	return value;
}


void SDUtility::lockIncrease(uint32_t &i, CRWLock &lock)
{
	lock.WriteLock();
	i++;
	lock.WriteUnlock();
}

void SDUtility::lockDecrease(uint32_t &i, CRWLock &lock)
{
	lock.WriteLock();
	i--;
	lock.WriteUnlock();
}

uint32_t SDUtility::lockGetValue(uint32_t &var, CRWLock &lock)
{
	uint32_t value;
	lock.ReadLock();
	value = var;
	lock.ReadUnlock();
	return value;
}

void SDUtility::lockSetValue(uint32_t &var, CRWLock &lock, uint32_t value)
{
	lock.WriteLock();
	var = value;
	lock.WriteUnlock();
}

	
std::string SDUtility::hash(const std::string& id)
{
	uint32_t result = 1;
	for(uint32_t i = 0; i < id.size(); i++)
	{
		result = 31 * result + id[i];
	}

	result = (result&0X7FFFFFFF) + 0X80000000;
	
	char buff[100]={0};
	sprintf(buff, "%02d", result%100);
		
	return string(buff);
}

int SDUtility::hash_gcid(std::string &str)
{
	int result = 1;
	
	if (str.empty())
		return 0;

	for (int i = 0; i < str.size(); i++)
		result = 31 * result + str[i];
 
	return abs(result);

}

// 需要兼容考虑纯数字的id 和 老的字符串形式的id
std::string SDUtility::user_postfix(std::string user_id)
{
	std::string postfix;
	
	// 如果后两位是数字则直接用数字，否则hash
	uint32_t len = user_id.length();

	if(len < 2)
	{
		postfix = "00";
	}
	else
	{
		if(isdigit(user_id[len-1]) && isdigit(user_id[len-2]))
		{
			postfix = user_id.substr(len-2,2);
		}
		else
		{
			postfix = hash(user_id);
		}
	}
	
	return postfix;

}


std::string SDUtility::group_postfix(std::string group_id)
{
	std::string postfix;

	if(group_id.length() < 2)
	{
		LOG4CPLUS_ERROR(logger,"group_id:" << group_id << " is too short");
		assert(0);
	}
	
	postfix = group_id.substr(group_id.length()-2,2);

	//LOG4CPLUS_DEBUG(logger,"group_id:" << group_id << " 's postfix:" << postfix);
	
	return postfix;
}
	

std::string SDUtility::message_postfix(std::string dst_id)
{
	std::string postfix = hash(dst_id);

	return postfix;
}

int SDUtility::hex2int(char chr)
{
	if(chr >= '0' && chr <= '9')
	{
		return chr - '0';
	}
	if(chr >= 'a' && chr <= 'f')
	{
		return chr - 'a' + 10;
	}
	if(chr >= 'A' && chr <= 'F')
	{
		return chr - 'A' + 10;
	}
	return 0;
}

bool SDUtility::hexstr2byte(std::string &binary_string, const std::string &hex_string)
{
	if ( 0 != (hex_string.length()%2) )
		return false;

	unsigned int buff_len = hex_string.length() / 2;
	char *buff_ptr = new char[buff_len];
	const char *hex_ptr = hex_string.c_str();
	char *temp_buffer_ptr = buff_ptr;

	for ( unsigned int i=0; i<buff_len; i++ )
	{
		*temp_buffer_ptr = 16*hex2int(*hex_ptr)+hex2int(*(hex_ptr+1));
		hex_ptr += 2;
		temp_buffer_ptr++;
	}

	binary_string.assign(buff_ptr, buff_len);

	delete [] buff_ptr;

	return true;
}

int SDUtility::strnicmp(const char *s1, const char *s2, size_t len) 
{ 
	unsigned char c1, c2; 
	
	c1 = 0; c2 = 0; 
	if (len) { 
		do { 
		c1 = *s1; c2 = *s2; 
		s1++; s2++; 
		//是否已到字符串的末尾或两字符串是否有空串,如果到了末尾或有空串,则比较完毕 
		if (!c1) 
			break; 
		if (!c2) 
			break;
		
		//如果没有,且字符串相等,则继续比较下个字符 
		if (c1 == c2) 
			continue; 
		
		//如果不相同,则同时转换为小写字符再进行比较 
		c1 = tolower(c1); 
		c2 = tolower(c2);
		
		//如果不相同,则比较完毕,否则继续 
		if (c1 != c2) 
			break; 
		}while (--len); 
	} 
	
	return (int)c1 - (int)c2; 
} 

unsigned int SDUtility::get_cur_millseconds()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_sec * 1000 + now.tv_usec / 1000;
}

int SDUtility:: GBKToUTF8(std::string &sStr)
{
        char pInBuf[sStr.size()+1];
        char pOutBuf[3*sStr.size()+1];

        snprintf(pInBuf, sStr.size()+1, "%s", sStr.c_str());
        G2U(pInBuf, strlen(pInBuf)+1, pOutBuf, 3*sStr.size()+1);
        sStr = pOutBuf;

        return 1;
}


int SDUtility:: UTF8ToGBK(std::string &sStr)
{
	char *pInBuf = new char[sStr.size()+1];
	char *pOutBuf = new char[3*sStr.size()+1];
	memset(pInBuf, 0, sStr.size()+1);
	memset(pOutBuf, 0, 3*sStr.size()+1);

	snprintf(pInBuf, sStr.size()+1, "%s", sStr.c_str());
	U2G(pInBuf, strlen(pInBuf)+1, pOutBuf, 3*sStr.size()+1);
	sStr = pOutBuf;

	delete [] pInBuf;
	delete [] pOutBuf;
	return 1;


}

int SDUtility::U2G(char *pszInBuf, int iInLen, char *pszOutBuf, int iOutLen)
{
	return code_convert("UTF-8","GBK", pszInBuf, iInLen, pszOutBuf, iOutLen);
}


int SDUtility::G2U(char *pszInBuf, int iInLen, char *pszOutBuf, int iOutLen)
{
        return code_convert("GBK", "UTF-8", pszInBuf, iInLen, pszOutBuf, iOutLen);
}

int SDUtility::code_convert(const char *pszFromCharSet, const char *pszToCharSet, char *pszInBuf, size_t iInLen, char *pszOutBuf, size_t iOutLen)
{
    iconv_t iConv;
    char **pin = &pszInBuf;
    char **pout = &pszOutBuf;

    iConv = iconv_open(pszToCharSet, pszFromCharSet);
    if(iConv < 0)
        return -1;

    memset(pszOutBuf, 0, iOutLen);
    if(iconv(iConv, pin, &iInLen, pout, &iOutLen) <= 0)
    {
        iconv_close(iConv);/*warining!!! this must be close, or there will be fatal error*/
        return -1;
    }

    iconv_close(iConv);
    return 0;
}


// java的string hashcode的c++实现
int SDUtility::hash_java( std::string &str )
{


	int h=0;
	int off = 0;   

	for (int i = 0; i < str.size(); i++) 
	{  
		h = 31*h + str[off++];  
	}  
	 
	if (h<0)  // 这个地方可能会是一个负数，需要转成正数，返回
	{
		return 0-h;
	} 
	else
	{
		return h;
	}



}

std::string SDUtility::format_time_all( time_t unix_time )
{
	/*std::string str_time;
	time_t currenttime = time(NULL);
	str_time  = format_day(currenttime);
	str_time  = str_time + " "+format_time(currenttime);
	return str_time;*/
	return format_time(unix_time);

}

int SDUtility::gethubIP(std::string &ip, const char *domain)
{
	char * *pptr;
	struct hostent *hptr;
	char buf[64];
	hptr = gethostbyname(domain);
	if (hptr == NULL)
	{
		//LOGGER_ERROR(g_logger, "gethostbyname() fail !");
		return -1;
	}

	pptr = hptr->h_addr_list;
	for (; *pptr != NULL; pptr++)
	{

		ip = inet_ntop(hptr->h_addrtype, *pptr, buf, sizeof(buf) );
		//LOGGER_WARN(g_logger,"get host succ:"<<ip <<" hostname:"<<domain);
		return 0;
	}

	return -1;

}

uint32_t SDUtility::ip2long( std::string str_ip )
{

	int position1 = str_ip.find(".");
	int position2 = str_ip.find(".", position1+1);
	int position3 = str_ip.find(".", position2+1);

	std::string ip00=str_ip.substr(0, position1);
	std::string ip11=str_ip.substr(position1+1, position2);
	std::string ip22=str_ip.substr(position2+1, position3);
	std::string ip33=str_ip.substr(position3+1);


	
	uint32_t ip0 = atouint32(ip00.c_str(),ip00.size());
	uint32_t ip1 = atouint32(ip11.c_str(),ip00.size());
	uint32_t ip2 = atouint32(ip22.c_str(),ip00.size());
	uint32_t ip3 = atouint32(ip33.c_str(),ip00.size());

	return (ip0<<24) + (ip1<<16) + (ip2<<8) + ip3;

}


unsigned char SDUtility::toHex(const unsigned char &x)
{
	
	return x > 9 ? x + 55: x + 48;
}

std::string SDUtility::URLEncode(const std::string &sIn)
{
	// cout << "size: " << sIn.size() << endl;
	string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{      
		unsigned char buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (unsigned char)sIn[ix] ) )
		{      
			buf[0] = sIn[ix];
		}
		else if ( isspace( (unsigned char)sIn[ix] ) )
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (unsigned char)sIn[ix] >> 4 );
			buf[2] = toHex( (unsigned char)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
};

bool SDUtility::URLDecode(std::string &url)
{
	char * pszUrl = strdup(url.c_str());
	unescape_url(pszUrl);
	url = pszUrl;
	free(pszUrl);

	return true;
};


void SDUtility::unescape_url(char *url)
{
	register int x,y;

	for (x=0,y=0; url[y]; ++x,++y) 
	{
		if((url[x] = url[y]) == '%') 
		{
			url[x] = x2c(&url[y+1]);
			y+=2;
		}
		else if ( url[y] == '+' )
		{
			url[x] = ' ';//space charset
		}
	}
	url[x] = '\0';
};

unsigned char SDUtility::x2c(const char *what)
{       // stolen from cgi-html
	register char digit;

	digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
	digit *= 16;
	digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
	return(digit);
}

bool SDUtility::json_array_append(std::string &old_array, std::string &elem)
{
	if(old_array.empty())
	{
		old_array = "[]";
	}
	std::map<std::string, std::string>::iterator iter;
       cJSON* JsonObj=cJSON_Parse(old_array.c_str());

        if (!JsonObj)
        {
                return false;
        }

        if (cJSON_Array != JsonObj->type)
        {
                return false;
        }

	cJSON *pjsonobj = cJSON_Parse(elem.c_str());
	cJSON_AddItemToArray(JsonObj, pjsonobj);
	
	char *pdata = cJSON_PrintUnformatted(JsonObj);
	old_array = pdata;
	free(pdata);
	
	cJSON_Delete(JsonObj);
	 return true;
}

void SDUtility::json_array_pop(std::string &old_array, std::string &elem)
{
   if(!old_array.empty())
   {
      std::vector<std::string> ars;
      std::string atoken = ",[] ";
      stringsplit(old_array, atoken, ars);
      int size = ars.size();
      old_array = "[";
      for(int i = 0; i < size; i++)
      {
         std::string rindex = ars.at(i);
         if(rindex != elem && !rindex.empty())
         {
            old_array.append(rindex);
            old_array.append(",");
         }
      }
      if(1 == old_array.size())
         old_array.append("]");
      else
         old_array= old_array.substr(0, old_array.length() - 1) + "]";
   }
}

std::string SDUtility::get_md5( std::string &src )
{

	unsigned char md5result[100]={0};
	unsigned char *pdata=NULL;
	pdata = MD5((const unsigned char*)src.c_str(), 
		(unsigned long)src.size(),
		(unsigned char *)md5result);

//const unsigned char *d, unsigned long n, unsigned char *md
	char buf[100]={0};
	for (int i = 0; i < 16; i++)
	{
		char tmp[10]={0};
		sprintf(tmp,"%2.2x",pdata[i]);
		strcat(buf,tmp);
	}

	//printf("%s\n",buf);
	std::string md5sign;
	md5sign = buf;
	return md5sign;

}

