/*
 * mycommon.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: hehenpan
 */

#include "mycommon.h"
#include "SDUtility.h"
#include <boost/algorithm/string.hpp>
static std::string g_str_cmd_filter("()");


static enum_running_mode running_mode = enum_runing_mode_production;

int set_running_mode(enum_running_mode mode)
{
	running_mode = mode;
	return 0;
}


enum_running_mode get_running_mode(void)
{
	return running_mode;
}
	
//
//
int printstring(std::string &str)
{
	std::string::iterator  iter;
	for(iter = str.begin(); iter!= str.end(); iter++)
	{
		byte data;
		data = *iter;
		printf("%.2X ",data);
	}
	printf(" ---\n");
	return 0;
}
//
int printstring2str(std::string &str_src, std::string &str_dst)
{
	std::string::iterator  iter;
	for(iter = str_src.begin(); iter!= str_src.end(); iter++)
	{
		std::string str_tail;
		char chardst[5] = {0};
		byte data;
		data = *iter;
		sprintf(chardst, "%.2X ", data);
		str_tail = chardst;
		str_dst = str_dst + str_tail;
		//printf("%.2X ",data);
	}
	return 0;
}

//
//void maketimeout_minutes(timespec *tsp, int minutes)
//{
//	struct timeval now;
//	gettimeofday(&now, NULL);
//	tsp->tv_sec = now.tv_sec;
//	tsp->tv_nsec = now.tv_usec * 1000;
//	tsp->tv_sec = tsp->tv_sec + minutes * 60;
//}
//
void maketimeout_seconds(timespec *tsp, int seconds)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	tsp->tv_sec = now.tv_sec;
	tsp->tv_nsec = now.tv_usec * 1000;
	tsp->tv_sec = tsp->tv_sec + seconds;
}
//
//
//int getcurrentimestr(char *ptime, int length)
//{
//	time_t mytime;
//	time(&mytime);
//	tm *tmtime = localtime(&mytime);
//	strftime(ptime, length, "%F %T",tmtime);
//	PRINTLOG_INFO("time is %s", ptime);
//	return 0;
//}




uint64_t atoi64(const char *pstr)
{
	return STRTOULL(pstr, NULL,0);
}

//
//
//uint64_t convert_local_to_utc_time(uint64_t local_time_t)
//{
//	return local_time_t - TM_GMTOFF;
//}



void EnableCoreFile(void)
{
	struct rlimit rl, rl_new;

	if (getrlimit(RLIMIT_CORE, &rl) == 0)
	{
		rl_new.rlim_cur = rl_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_CORE, &rl_new))
		{
			// failed.
			rl_new.rlim_cur = rl_new.rlim_max = rl.rlim_max;
			setrlimit(RLIMIT_CORE, &rl_new);
		}
	}

	// Check again
	if (getrlimit(RLIMIT_CORE, &rl) || rl.rlim_cur == 0)
	{
		//PRINTLOG_ERROR("Failed to ensure core file creation.");
		exit(1);
	}
	else
	{
		//PRINTLOG_INFO("enable dore dump finished");
	}
}

void EnableFileLimit(void)
{
	struct rlimit rl, rl_new;

	if (getrlimit(RLIMIT_NOFILE, &rl) == 0)
	{
		rl_new.rlim_cur = rl_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_NOFILE, &rl_new))
		{
			// failed.
			rl_new.rlim_cur = rl_new.rlim_max = rl.rlim_max;
			setrlimit(RLIMIT_NOFILE, &rl_new);
		}
	}

	// Check again
	if (getrlimit(RLIMIT_NOFILE, &rl) || rl.rlim_cur == 0)
	{
		//PRINTLOG_ERROR("Failed to ensure core file creation.");
		exit(1);
	}
	else
	{
		//PRINTLOG_INFO("enable dore dump finished");
	}
}




void erasestr(std::string &str)
{
	size_t pos = str.find('\r');
	if (pos == std::string::npos)
	{
		return;
	}
	str.erase(pos);
	
}


int pre_process_shell_cmd(std::string &str_cmd)
{
	std::string str_insert;
	str_insert = "\\";
	//std::string::iterator iter_filter;
	BOOST_FOREACH(char filter, g_str_cmd_filter)
	{
		while (true)
		{
			int pos = str_cmd.find(filter);
			if (pos == std::string::npos)
			{
				break;
			}
			
			str_cmd.insert(pos, str_insert);
			
		}
		
	}


	return 0;
	
}

int process_path_erase( std::string &str_file_path )
{

	if (str_file_path.size() == 0)
	{
		return -1;
	}
	if (str_file_path[str_file_path.size() -1 ] == '/')
	{
		str_file_path.erase(str_file_path.size()-1);
	}
	return 0;
	
	
}

void map2string( std::string &str_rtn, std::map<std::string, std::string> &map_src )
{

	/*std::map<std::string, std::string>::iterator iter;
	for (iter = map_src.begin(); iter != map_src.end(); iter++)
	{
		str_rtn = str_rtn+iter->first+"="+iter->second+"&";
	}*/
	std::map<std::string, std::string>::reverse_iterator riter;
	for (riter = map_src.rbegin(); riter != map_src.rend(); riter++)
	{
		str_rtn = str_rtn+riter->first+"="+riter->second+"&";
	}
	// 去掉最后一个& 符号
	if ( ! str_rtn.empty())
	{
		str_rtn.erase(str_rtn.length()-1);
	}

}

void stringsplit(std::string &str_src, std::string &token, std::vector<std::string> &vc)
{
	boost::split( vc, str_src, boost::is_any_of(token));
}

void string2map( std::string &str_src, std::map<std::string, std::string> &map_rtn )
{
	std::vector<std::string> vec;
	boost::split( vec, str_src, boost::is_any_of("&") );
	BOOST_FOREACH(std::string str_item, vec)
	{
		std::vector<std::string> vec_kv;
		boost::split(vec_kv, str_item, boost::is_any_of("="));
		if (vec_kv.size() != 2)
		{
			continue;
		}
		std::string key = vec_kv[0];
		std::string value = vec_kv[1];
		boost::trim(key);
		boost::trim(value);
		map_rtn.insert(make_pair(key, value));
		
	}

}

void string2map_urldecode( std::string &str_src, std::map<std::string, std::string> &map_rtn )
{
	std::vector<std::string> vec;
	boost::split( vec, str_src, boost::is_any_of("&") );
	BOOST_FOREACH(std::string str_item, vec)
	{
		std::vector<std::string> vec_kv;
		boost::split(vec_kv, str_item, boost::is_any_of("="));
		if (vec_kv.size() != 2)
		{
			continue;
		}
		std::string key = vec_kv[0];
		std::string value = vec_kv[1];
		boost::trim(key);
		boost::trim(value);
		SDUtility::URLDecode(value);
		map_rtn.insert(make_pair(key, value));

	}

}

std::string get_value( std::string key, std::map<std::string, std::string> *map_get )
{
	std::map<std::string, std::string>::iterator iter;
	iter = map_get->find(key);
	if ( iter != map_get->end() )
	{
		return iter->second;
	}
	else
	{
		return "";
	}
	
}


std::string get_vaule( std::string key, std::map<std::string, std::string> &map_get )
{
	std::map<std::string, std::string>::iterator iter;
	iter = map_get.find(key);
	if ( iter != map_get.end() )
	{
		return iter->second;
	}
	else
	{
		return "";
	}
	
}



void hton32(uint32_t *a)
{
	union 
	{
		uint32_t i;
		char c[4];

	} u;
	u.i = *a;
	char *p = (char *)a;
	p[0] = u.c[3];
	p[1] = u.c[2];
	p[2] = u.c[1];
	p[3] = u.c[0];
	//return r.i;
}










