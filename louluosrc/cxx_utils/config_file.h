/*
 * #################################################################################################
 *	All rights reserved.
 *	Author: manialuo <1514louluo@sina.com>
 *	Date: 2015-08-19
 *	License: MIT
 *	Desc: 配置文件解析,支持 [PREFIX] K=V 和 PREFIX.K V 两种格式(注意PREFIX.K V 中间是空格 or '\t')
 * #################################################################################################
 */
#ifndef _UTILS_CONFIGFILE_H
#define _UTILS_CONFIGFILE_H

#include <vector>
#include <string>
#include <cstring>
#include <cstdio>

#ifndef _ENGINE_VEC  //_ENGINE_VEC: vec形式二分查找的形式存储, 默认以unordermap来存储
#include "tmlboost/unordered_map.hpp"
#endif

namespace cxx_utils
{

	/*
	 * 配置文件每项的存储 key value
	 */
	class ConfigItem
	{
		public:
			ConfigItem(const std::string& s1, const std::string& s2)
			{
				m_key = s1;
				m_value = s2;
			}

#ifdef _ENGINE_VEC  
			friend bool operator< (const ConfigItem& s1, const ConfigItem& s2)
			{
				return s1.m_key < s2.m_key;
			}
#endif

			std::string m_key;
			std::string m_value;
	};

	class ConfigFile
	{
	  public:
		  ConfigFile(const char   *file_name);
		  ConfigFile(const std::string &file_name);
		  ConfigFile();
		  ~ConfigFile();

		 /*
		 *  Desc:  读取配置文件
		 *  @param file_name  文件路径
		 *  @return  false true
		 */
		  bool load(const char* file_name = NULL);

		 /*
		 *  Desc:  以string的形式读取某项
		 *  @param key
		 *  @param default_value 表示没有找到时的默认选项
		 *  @return  找到的value 或者是默认value
		 */
		  std::string get_string(const char* key, const std::string& default_value="") const;

		 /*
		 *  Desc:  以int的形式读取某项
		 *  @param key
		 *  @param default_value 表示没有找到时的默认选项
		 *  @return  找到的value 或者是默认value
		 */
		  int get_int(const char *key, int default_value=0) const;

		 /*
		 *  Desc:  插入某项
		 *  @param ConfigItem
		 *  @return
		 */
		  void insert_item(const ConfigItem&);

#ifdef _ENGINE_VEC  
		 /*
		 *  Desc:  拷贝vec项
		 *  @param std::vector<ConfigItem>
		 *  @return
		 */
		  void copy_item_data(std::vector<ConfigItem> &vecItem);
#endif

	  private:
		  bool loadline(char* buffer); //解析某行
		  void trim(std::string&);  //去掉首位的空格 and '\t'

	  private:
#ifdef _ENGINE_VEC  
		  std::vector<ConfigItem> m_content; //所有配置项放入此vec
#else
      boost::unordered_map<std::string, std::string> m_content; //所有配置项放入此map
#endif
		  std::string m_file_name;           //要读取的配置文件名
		  std::string m_prefix;              // [XX] K=V 这种形式下的XX
		  
      static const int kMaxLineLength = 1024; //一行最大解析1024个字节长度的配置
	};
}

#endif //_UTILS_CONFIGFILE_H

