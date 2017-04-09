#ifndef RVSGEO_H
#define RVSGEO_H

#include <errno.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <list>
#include <vector>
#include <set>

#include "stdlib.h"
#include "string.h"
#include "sys/resource.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <mysql.h>
#include <signal.h>

//#include "tinyxml.h"

// ------------BOOST LIB HEADER-------------------
#include "boost/smart_ptr.hpp"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "boost/bimap.hpp"
#include "boost/foreach.hpp"
#include "boost/algorithm/string.hpp"

#define STRTOD(PTR, END_PTR) ((PTR == NULL)? DBL_MAX: strtod(PTR, END_PTR))
#define STRTOULL(PTR, END_PTR, BASE) ((PTR == NULL)? ULLONG_MAX: strtoull(PTR, END_PTR, BASE))

#define MAX_BUFF_LENGTH   1024

//
//
//int loadconfig(char *filename);
//
//int initlog4cxx(const char *config);
//
int printstring(std::string &str);
//
int printstring2str(std::string &str_src, std::string &str_dst);
//
void erasestr(std::string &str);
//void maketimeout_minutes(timespec *tsp, int minutes);
//
//void maketimeout_seconds(timespec *tsp, int seconds);


//typedef   unsigned char  BYTE;
typedef   unsigned char  byte;
typedef   unsigned short word;
typedef   unsigned short WORD;
typedef   unsigned int   dword;
typedef   unsigned int   DWORD;

typedef std::string string_type;
typedef std::vector<string_type> string_vector;
typedef std::pair<string_type, string_type> string_pair;
typedef std::vector<string_pair> string_pair_vector;
typedef std::pair<string_type, double> string_score_pair;
typedef std::vector<string_score_pair> string_score_vector;
typedef std::set<string_type> string_set;

void hton32(uint32_t *a);



#define SCOPED_LOCK_LOCKED   0
#define SCOPED_LOCK_UNLOCKED 1
class scopedlock
{
public:

	scopedlock(pthread_mutex_t *p_mutex)
	{
		m_p_mutex = p_mutex;
		pthread_mutex_lock(m_p_mutex);
		m_lock_tag = SCOPED_LOCK_LOCKED;
	}

	~scopedlock()
	{
		if(m_lock_tag == SCOPED_LOCK_LOCKED)
		{
			pthread_mutex_unlock(m_p_mutex);
		}
	}

	void unlock()
	{
		if(m_lock_tag == SCOPED_LOCK_LOCKED)
		{
			pthread_mutex_unlock(m_p_mutex);
			m_lock_tag = SCOPED_LOCK_UNLOCKED;
		}
	}

private:
	int m_lock_tag;
	pthread_mutex_t *m_p_mutex;
};


int getcurrentimestr(char *ptime, int length);


#define STRTOULL(PTR, END_PTR, BASE) ((PTR == NULL)? ULLONG_MAX: strtoull(PTR, END_PTR, BASE))

#define MACRO_MEM_ZERO(data)  memset(&data, 0, sizeof(data))

class MyMutex
{
public:
	MyMutex(){ pthread_mutex_init(&m_mutex, NULL);}
	~MyMutex(){pthread_mutex_destroy(&m_mutex);}
	pthread_mutex_t *get(void){ return &m_mutex;}
private:
	pthread_mutex_t m_mutex;
};


uint64_t atoi64(const char *pstr);


#define APPEND_STREAM(str, data)   str.append((char *)&data, sizeof(data))


#define  MAX(a,  b)  (  (a)>(b)?(a):(b) )
#define  MIN(a,  b)  (  (a)<(b)?(a):(b) )




#define    ONE_THOUSAND   1000
#define    TM_GMTOFF   28800  //8 hour tick recorded with second unit


void maketimeout_seconds(timespec *tsp, int seconds);



void EnableCoreFile(void);
void EnableFileLimit(void);

int pre_process_shell_cmd(std::string &str_cmd);

// 如果末尾有 / ，则默认去掉
int process_path_erase(std::string &str_file_path);


enum enum_running_mode
{
	enum_runing_mode_production=0,
	enum_runing_mode_test=1,
};

int set_running_mode(enum_running_mode mode);
enum_running_mode get_running_mode(void);
#define SIMULATION_DELAY  \
	if (get_running_mode() == enum_runing_mode_test) \
{ \
	SYS_LOG_INFO("SIMULATION_DELAY delay 1s"); \
	sleep(1); \
} \
	else \
{ \
	; \
}


std::string get_value( std::string key, std::map<std::string, std::string> *map_get );
void map2string(std::string &str_rtn, std::map<std::string, std::string> &map_src);
void string2map(std::string &str_src, std::map<std::string, std::string> &map_rtn);
void string2map_urldecode( std::string &str_src, std::map<std::string, std::string> &map_rtn );
std::string get_vaule(std::string key, std::map<std::string, std::string> &map_get);
void stringsplit(std::string &str_src, std::string &token, std::vector<std::string> &vc);

#endif












