#include "libcurlclient.h"


IMPL_LOGGER(libcurlclient, logger);


libcurlclient::libcurlclient(void)
{
}

libcurlclient::~libcurlclient(void)
{
}


long libcurlclient::writer_response(void *data, int size, int nmemb, void *content)
{
	long sizes = size * nmemb;
	std::string temp;
	temp.append((char *)data,sizes);
	*(std::string *)content = *(std::string *)content + temp; 
	//printf("%s\n",temp.c_str());
	//memcpy(content, data, sizes);
	return sizes;
}

long libcurlclient::writer_header(void *data, int size, int nmemb, void *content)
{
	long sizes = size * nmemb;
	std::string temp;
	temp.append((char *)data,sizes);
	*(std::string *)content = *(std::string *)content + temp; 
	//printf("%s\n",temp.c_str());
	//memcpy(content, data, sizes);
	return sizes;
}

void libcurlclient::INIT_LIBURLCLIENT( void )
{

	CURLcode code;
	code = curl_global_init(CURL_GLOBAL_ALL);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to init libcurl module, ready to quit process");
		sleep(1);
		exit(0);
	}
}

int libcurlclient::http_request( std::string &str_url, std::string &str_post_param )
{
	
	CURLcode code;
	CURL *curl = curl_easy_init();
	if(curl == NULL)
	{
		MY_LOGERROR("failed to init libcurl");
		return  -1;
	}
	//char *purl="www.sina.com.cn";
	//purl = "http://websvr.niu.xunlei.com:80/clientLoginGame.webGameLogin";
	//char *postfield = "username=hehenpanxp@163.c&pwd=3ed80171b1f4ab825f2038fc203c887c&userid=39450075933945&copartnerid=0101&gameid=000069&sid=2035813&loginurl=http:%2F%2Fxx&serverid=250025&sign=4071e5a6e06345fde19db67a4d4c868d&clientip=192.168.101.51";
	code = curl_easy_setopt(curl, CURLOPT_URL, str_url.c_str());
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set url opt");
		curl_easy_cleanup(curl);
		return -1;
	}

	code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_post_param.c_str());
	if (code != CURLE_OK)
	{
		MY_LOGERROR("faile to set url postfiled");
		curl_easy_cleanup(curl);
		return -1;
	}

	code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, libcurlclient::writer_response);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set callback func");
		curl_easy_cleanup(curl);
		return -1;
	}

	code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_str_response_body);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set writedata param");
		curl_easy_cleanup(curl);
		return -1;
	}

	code = curl_easy_setopt(curl, CURLOPT_HEADERDATA, &m_str_response_header);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set CURLOPT_HEADERDATA param");
		curl_easy_cleanup(curl);
		return -1;
	}
	code = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, &writer_header);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set CURLOPT_HEADERFUNCTION param");
		curl_easy_cleanup(curl);
		return -1;
	}
	/*code = curl_easy_setopt(curl,CURLOPT_TIMEOUT, 1);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set CURLOPT_TIMEOUT param");
		curl_easy_cleanup(curl);
		return -1;
	}*/



	CURLcode res = curl_easy_perform(curl);
	/* Check for errors */ 
	if(res != CURLE_OK)
	{
		MY_LOGERROR("curl_easy_perform() failed: "<<curl_easy_strerror(res));
		return -1;
	}

	/* always cleanup */ 
	curl_easy_cleanup(curl);
	
	return 0;

}

int libcurlclient::http_request_timeout_seconds( std::string &str_url, std::string &str_post_param, 
												 int timeout )
{
	//CURLOPT_TIMEOUT
	//std::string str_result;
	CURLcode code;
	CURL *curl = curl_easy_init();
	if(curl == NULL)
	{
		MY_LOGERROR("failed to init libcurl");
		return  -1;
	}

	code = curl_easy_setopt(curl, CURLOPT_URL, str_url.c_str());
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set url opt");
		curl_easy_cleanup(curl);
		return -1;
	}

	code = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_post_param.c_str());
	if (code != CURLE_OK)
	{
		MY_LOGERROR("faile to set url postfiled");
		curl_easy_cleanup(curl);
		return -1;
	}

	code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, libcurlclient::writer_response);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set callback func");
		curl_easy_cleanup(curl);
		return -1;
	}

	code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_str_response_body);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set writedata param");
		curl_easy_cleanup(curl);
		return -1;
	}
	//code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
	code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, &timeout);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set writedata param");
		curl_easy_cleanup(curl);
		return -1;
	}
	

	code = curl_easy_setopt(curl, CURLOPT_HEADERDATA, &m_str_response_header);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set CURLOPT_HEADERDATA param");
		curl_easy_cleanup(curl);
		return -1;
	}
	code = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, &writer_header);
	if (code != CURLE_OK)
	{
		MY_LOGERROR("failed to set CURLOPT_HEADERFUNCTION param");
		curl_easy_cleanup(curl);
		return -1;
	}



	CURLcode res = curl_easy_perform(curl);
	/* Check for errors */ 
	if(res != CURLE_OK)
	{
		MY_LOGERROR("curl_easy_perform() failed: "<<curl_easy_strerror(res)<<"  url="<<str_url<<"  params="<<str_post_param);
		return -1;
	}

	/* always cleanup */ 
	curl_easy_cleanup(curl);
	return 0;
}
