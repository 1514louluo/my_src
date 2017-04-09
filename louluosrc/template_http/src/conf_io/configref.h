#ifndef _CONFIGREF_H
#define _CONFIGREF_H

#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include "logdef.h"

class config_item
{
public:
	config_item(const std::string& s1, const std::string& s2)
	{
		m_key = s1;
		m_value = s2;
	}

	std::string m_key;
	std::string m_value;

	friend bool operator< (const config_item& s1, const config_item& s2)
	{
		return s1.m_key < s2.m_key;
	}
};

class configref
{
	static const int max_path = 1024;
	static const int max_line_length = 1024;
public:
	configref(const char* file_name);
	configref();
	virtual ~configref();

	bool load(const char* file_name = NULL);
	std::string getString(const char* key, const std::string& default_value="") const;
	int getInt(const char *key, int default_value=0) const;
	void insertItem(const config_item&);

	void copyItemData(std::vector<config_item> &vecItem);
	
private:
	char m_file_name[max_path];
	std::vector<config_item> m_content;

	bool loadLine(char* buffer);
	void trim(std::string&);

	DECL_LOGGER(logger);
};

#endif

