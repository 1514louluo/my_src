#include <algorithm>
#include "ConfigFile.h"

namespace QQMUSIC_UTILS
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

	char buffer[max_line_length], *p_ret;
    FILE* fp = fopen(m_file_name.c_str(), "r");

	if(!fp)
	{
		return false;
	}

	m_content.clear();
	for(;;)
	{
		p_ret = fgets(buffer, max_line_length, fp);
        if(!p_ret || feof(fp))
		{
			break;
		}
		loadLine(buffer);
	}
	fclose(fp);
	return true;
}

bool ConfigFile::loadLine(char* buffer)
{
    //首先判断改行是不是注释
    char *p_lf = strrchr(buffer, '\n');  //该行的结束位置
    string strline = string(buffer, 0, p_lf - buffer); // 该行的内容
    trim(strline);
    if(strline.empty()) return true; //空行
    else if(0 == strline.find_first_of('#'))  return true;//如果是注释

    //判断该行是不是 [xxx]
    if('[' == strline[0] && ']' == strline[strline.size()-1])
    {
        m_prefix = strline.substr(1, strline.size()-2);
        return true;
    }
    //如果不是 [XXX] 那么判断是否有 = 号
    size_type pos = strline.find('=', 0);
    if(string::npos != pos) //有 = 号
	{
        string key = m_prefix + strline.substr(0, pos);
        string value = strline.substr(pos+1, strline.size() - pos - 1);
		trim(key);
		trim(value);
        if (!key.empty())
            insertItem(ConfigItem(key, value));
	}
    else  // 没有 = 号
	{
        size_type wpos = strline.find(0x20, 0);
        if(string::npos != wpos)
        {
            string key = strline.substr(0, wpos);
            string value = strline.substr(wpos+1, strline.size() - wpos - 1);
            trim(key);
            trim(value);
            if (!key.empty())
                insertItem(ConfigItem(key, value));
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
}

void ConfigFile::insertItem(const ConfigItem& item)
{
    vector<ConfigItem>::iterator it_find
		= lower_bound(m_content.begin(), m_content.end(), item);

	if(it_find == m_content.end() || it_find->m_key != item.m_key)
	{
        cout<<"insert [" << item.m_key << "," << item.m_value << "]";
		m_content.insert(it_find, item);
	}
}

string ConfigFile::getString(const char* key, const std::string& default_value) const
{
    vector<ConfigItem>::const_iterator it_find
        = lower_bound(m_content.begin(), m_content.end(), ConfigItem(key, ""));

	if(it_find != m_content.end() && it_find->m_key == key)
	{
        cout<<"getString" << key<<"="<<it_find->m_value;
		return it_find->m_value;
	} else {
        cout<<"getString error!! item '" << key << "' not found!! use default value "<<default_value<<endl;
		return default_value;
	}

	return "";
}

int ConfigFile::getInt(const char *key, int default_value) const
{
    vector<ConfigItem>::const_iterator it_find
        = lower_bound(m_content.begin(), m_content.end(), ConfigItem(key, ""));

	if(it_find != m_content.end() && it_find->m_key == key)
	{
        cout<<"getInt" << key<<"="<<it_find->m_value;
        return atoi(it_find->m_value.c_str());
	} else {
        cout<<"getInt error!! item '" << key << "' not found!! use default value "<<default_value<<endl;
        return default_value;
	}

	return 0;
}

void ConfigFile::copyItemData(std::vector<ConfigItem> &vecItem)
{
	vecItem.clear();
	vecItem.insert(vecItem.begin(), m_content.begin(), m_content.end());
}

}
