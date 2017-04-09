/*
 * ¹«ÓÃº¯Êý
 *
 * author: 
 *     Zhang Haijian
 *
 * latest update: 
 *     2009-09-04
 *
 */ 

#ifndef SDUtility_H
#define SDUtility_H


#include <string>
#include <stdint.h>
#include "ThreadSync.h"
#include <sys/time.h>
#include "SDLogger.h"
#include <net/if.h>  
#include <arpa/inet.h>

#include <stdio.h>  
#include <sys/ioctl.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <netdb.h>

#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <stdlib.h>
#include "mycommon.h"
#include "md5.h"
#include "cJSON.h"

class SDUtility
{
public:
	static std::string str2hex(const char* buf, int size);
	static int hex2int(char chr);
	static bool hexstr2byte(std::string &binary_string, const std::string &hex_string);
		
	static int strnicmp(const char *s1, const char *s2, size_t len);

	static std::string format(int i);
	static std::string format(uint32_t i);
	static std::string format(uint64_t i);
	static std::string format_time(time_t unix_time);
	static std::string get_current_time();
	static std::string format_day(time_t unix_time);
	static std::string format_day_fake(time_t unix_time);
	static std::string get_local_datestring();
	static std::string format_time_all(time_t unix_time);
	static uint32_t  get_remind_secs_from_now2tomorrow(void);

	static std::string format_ip(uint32_t ip);

	static uint32_t    ip2long(std::string str_ip);

	static uint8_t  atouint8(char* ptr, int32_t len);
	static uint32_t  atouint32(const char* ptr, int32_t len);
	static uint64_t  atouint64(char* ptr, int32_t len);
	static uint64_t atouint64(const char* ptr, int32_t len);

	static uint32_t atouint32(std::string &str);

	static uint64_t atouint64(std::string &str);
	
	static void lockIncrease(uint32_t &i, CRWLock &lock);
	static void lockDecrease(uint32_t &i, CRWLock &lock);
	static uint32_t lockGetValue(uint32_t &var, CRWLock &lock);
	static void lockSetValue(uint32_t &var, CRWLock &lock, uint32_t value);
	static int gethubIP(std::string &ip, const char *domain);
	static std::string hash(const std::string& id);
	static std::string ip_search(void);
	static int hash_gcid(std::string &gcid);
	static int hash_java(std::string &str);

	static std::string user_postfix(std::string user_id);

	static std::string group_postfix(std::string group_id);
		
	static std::string message_postfix(std::string dst_id);

	static unsigned int get_cur_millseconds();

	static int GBKToUTF8(std::string &sStr);
	static int UTF8ToGBK(std::string &sStr);
	
	static int G2U(char *pszInBuf, int iInLen, char *pszOutBuf, int iOutLen);
	static int U2G(char *pszInBuf, int iInLen, char *pszOutBuf, int iOutLen);

	static int code_convert(const char *pszFromCharSet, const char *pszToCharSet, char *pszInBuf, size_t iInLen, char *pszOutBuf, size_t iOutLen);

	static std::string URLEncode(const std::string &sIn);
	static bool URLDecode(std::string &url);

	static bool  json_array_append(std::string &old_array, std::string &elem);
	static void  json_array_pop(std::string &old_array, std::string &elem);
	static std::string get_md5( std::string &src );


private:

	static unsigned char toHex(const unsigned char &x);
	static void unescape_url(char *url);
	static unsigned char x2c(const char *what);
	
protected:
	DECL_LOGGER(logger);

};


#endif 

