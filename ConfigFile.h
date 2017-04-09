/*
 * #################################################################################################
 *	Copyright (C) tencent 2015
 *	All rights reserved.
 *	Author: manialuo <1514louluo@sina.com>
 *	Date: 2015-08-19
 *	License: MIT
 *	Desc: 配置文件解析,支持 [PREFIX] K=V 和 PREFIX.K V 两种格式(注意PREFIX.K V 中间是空格)
 * #################################################################################################
 */
#ifndef _QQMUSIC_UTILS_CONFIGFILE_H
#define _QQMUSIC_UTILS_CONFIGFILE_H

#include <vector>
#include <string>
#include <cstring>
#include <cstdio>

namespace QQMUSIC_UTILS
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

    friend bool operator< (const ConfigItem& s1, const ConfigItem& s2)
    {
        return s1.m_key < s2.m_key;
    }

    std::string m_key;
    std::string m_value;
};

class ConfigFile
{
    static const int max_line_length = 1024; //一行最大解析1024个字节长度的配置
public:
    ConfigFile(const char   *file_name);
    ConfigFile(const string &file_name);
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
    std::string getString(const char* key, const std::string& default_value="") const;
    /*
    *  Desc:  以int的形式读取某项
    *  @param key
    *  @param default_value 表示没有找到时的默认选项
    *  @return  找到的value 或者是默认value
    */
    int getInt(const char *key, int default_value=0) const;

    /*
    *  Desc:  插入某项
    *  @param ConfigItem
    *  @return
    */
    void insertItem(const ConfigItem&);
    /*
    *  Desc:  拷贝vec项
    *  @param std::vector<ConfigItem>
    *  @return
    */
    void copyItemData(std::vector<ConfigItem> &vecItem);

private:
    bool loadLine(char* buffer); //解析某行
    void trim(std::string&);  //去掉首位的空格

private:
    std::vector<ConfigItem> m_content; //所有配置项放入此vec
    std::string m_file_name;           //要读取的配置文件名
    std::string m_prefix;              // [XX] K=V 这种形式下的XX
};
}

#endif //_QQMUSIC_UTILS_CONFIGFILE_H

