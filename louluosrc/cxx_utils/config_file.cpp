#include <algorithm>
#include "config_file.h"

namespace cxx_util
{
  using namespace std;

  ConfigFile::ConfigFile(const char* file_name)
  {
    m_file_name = file_name;
    m_prefix = "";
  }

  ConfigFile::ConfigFile(const string &file_name)
  {
    m_file_name = file_name;
    m_prefix = "";
  }

  ConfigFile::ConfigFile()
  {
    m_prefix = "";
  }

  ConfigFile::~ConfigFile()
  {
    m_content.clear();
  }
  bool ConfigFile::load(const char* file_name)
  {
    if(file_name != NULL)
    {
      m_file_name = file_name;
    }

    char buffer[kMaxLineLength], *p_ret;
    FILE* fp = fopen(m_file_name.c_str(), "r");

    if(!fp)
    {
      return false;
    }

    m_content.clear();
    for(;;)
    {
      p_ret = fgets(buffer, kMaxLineLength, fp);
      if(!p_ret || feof(fp))
      {
        break;
      }
      loadline(buffer);
    }
    fclose(fp);
    return true;
  }

  bool ConfigFile::loadline(char* buffer)
  {
    //首先判断改行是不是注释
    string strline = string(buffer); // 该行的内容
    string::size_type pos = strline.find('\r', 0);
    if(std::string::npos == pos) //no '\r'
      pos = strline.find('\n', 0); // find '\n'

    strline = strline.substr(0, pos);
    trim(strline);
    if(strline.empty()) return true; //空行
    else if(0 == strline.find_first_of('#'))  return true;//如果是注释

    //判断该行是不是 [xxx]
    if('[' == strline[0] && ']' == strline[strline.size()-1])
    {
      m_prefix = strline.substr(1, strline.size()-2);
      m_prefix.append(".");
      return true;
    }
    //如果不是 [XXX] 那么判断是否有 = 号
    pos = strline.find('=', 0);
    if(std::string::npos != pos) //有 = 号
    {
      string key = m_prefix + strline.substr(0, pos);
      string value = strline.substr(pos+1, strline.size() - pos - 1);
      trim(key);
      trim(value);
      if (!key.empty()) insert_item(ConfigItem(key, value));
    }
    else  // 没有 = 号
    {
      pos = strline.find('\t', 0);
      if(std::string::npos == pos)  pos = strline.find(0x20, 0);
      if(std::string::npos != pos)
      {
        string key = strline.substr(0, pos);
        string value = strline.substr(pos+1, strline.size() - pos - 1);
        trim(key);
        trim(value);
        if (!key.empty()) insert_item(ConfigItem(key, value));
      }
      else return false;
    }
    return true;
  }

  void ConfigFile::trim(string& s)
  {
    int bpos = s.find_first_not_of(0x20); //非空格
    int epos = s.find_last_not_of(0x20);  //非空格
    if(bpos < 0 || epos < 0)
      s.clear();
    else
      s = string(s, bpos, epos - bpos + 1);
    bpos = s.find_first_not_of('\t'); //非'\t'
    epos = s.find_last_not_of('\t');  //非'\t'
    if(bpos < 0 || epos < 0)
      s.clear();
    else
      s = string(s, bpos, epos - bpos + 1);

  }

  void ConfigFile::insert_item(const ConfigItem& item)
  {
#ifndef _ENGINE_VEC  
    //  m_content[item.m_key] = item.m_value;
    m_content.insert(boost::unordered_map<std::string, std::string>::value_type(item.m_key, item.m_value));
#else
    vector<ConfigItem>::iterator it_find
      = lower_bound(m_content.begin(), m_content.end(), item);

    if(it_find == m_content.end() || it_find->m_key != item.m_key)
    {
      //这里用了insert 这里可能有所谓的效率问题，但是配置文件本身就不应该很大，而且把这种有点低效率的事情在初始化来完成，不会影响业务的性能
      m_content.insert(it_find, item); 
    }
#endif
  }

  string ConfigFile::get_string(const char* key, const std::string& default_value) const
  {
#ifndef _ENGINE_VEC  
    boost::unordered_map<std::string, std::string>::const_iterator it_find = m_content.find(key);
    if(m_content.end() == it_find) //空
    {
      return default_value;
    }
    else
    {
      return it_find->second;
    }
#else
    vector<ConfigItem>::const_iterator it_find
      = lower_bound(m_content.begin(), m_content.end(), ConfigItem(key, ""));

    if(it_find != m_content.end() && it_find->m_key == key)
    {
      return it_find->m_value;
    } 
    else 
    {
      return default_value;
    }

    return "";
#endif
  }

  int ConfigFile::get_int(const char *key, int default_value) const
  {
#ifndef _ENGINE_VEC  
    boost::unordered_map<std::string, std::string>::const_iterator it_find = m_content.find(key);
    if(m_content.end() == it_find) //空
    {
      return default_value;
    }
    else
    {
      return atoi(it_find->second.c_str());
    }
#else
    vector<ConfigItem>::const_iterator it_find
      = lower_bound(m_content.begin(), m_content.end(), ConfigItem(key, ""));

    if(it_find != m_content.end() && it_find->m_key == key)
    {
      return atoi(it_find->m_value.c_str());
    } 
    else 
    {
      return default_value;
    }

    return 0;
#endif
  }

#ifdef _ENGINE_VEC  
  void ConfigFile::copy_item_data(std::vector<ConfigItem> &vecItem)
  {
    vecItem.clear();
    vecItem.insert(vecItem.begin(), m_content.begin(), m_content.end());
  }
#endif

}
