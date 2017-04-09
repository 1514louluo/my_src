#include <algorithm>
#include "configref.h"

using namespace std;

IMPL_LOGGER(configref, logger)

configref::configref(const char* file_name)
{
	strcpy(m_file_name, file_name);
}

configref::configref()
{
	memset(m_file_name, 0, sizeof(m_file_name));
}

configref::~configref()
{
	m_content.clear();
}
/*
bool configref::load(void)
{
	ssize_t read;
	size_t len;
	char *line = NULL;
	FILE *fp = fopen(m_file_name, "r");
	if(NULL == fp) return false;
	while ((read = getline(&line, &len, fp)) != -1) 
	{
		char *tmp = line;
        	while(*tmp++)
		{
               		if('\n' == *tmp || '\r' == *tmp)
                        	*tmp = '\0';
		}
		tmp = line;
        	while(*tmp)
        	{
			if('=' == *tmp)
			{
				std::string key;
				std::string value;
				char *noblank = tmp-1;
				while(' ' == *noblank || '\t' == *noblank){ noblank--; }
				key.append(line, noblank-line+1);
				noblank = tmp + 1;
				while(' ' == *noblank || '\t' == *noblank){ noblank++; }
				value.append(noblank);

				if (key.length()>0 && key.find_first_of('#')!=0)
		    			insertItem(config_item(key, value));
			}
                	tmp++;
		}
        }
        if (line)	free(line);
        fclose(fp);		
	return true;
}
*/
bool configref::load(const char* file_name)
{
	if(file_name != NULL)
	{
		strcpy(m_file_name, file_name);
	}

	char buffer[max_line_length], *p_ret;
	FILE* fp = fopen(m_file_name, "r");

	if(!fp)
	{
		return false;
	}

	m_content.clear();
	for(;;)
	{
		p_ret = fgets(buffer, max_line_length, fp);
		if(!p_ret)
		{
			break;
		}
		loadLine(buffer);
	}
	fclose(fp);
	return true;
}

bool configref::loadLine(char* buffer)
{
	char *p_eq = strchr(buffer, '=');
	if(p_eq)
	{
		int pos = p_eq - buffer;
		char *p_LF = strrchr(buffer, '\n');
		string key = string(buffer, 0, pos);
		string value = string(buffer, pos + 1, p_LF - p_eq - 1);
		trim(key);
		trim(value);
                if (key.length()>0 && key.find_first_of('#')!=0)
		    insertItem(config_item(key, value));
	}
	else
	{
		return false;
	}
	return true;
}

void configref::trim(string& s)
{
	int bpos = s.find_first_not_of(0x20);
	int epos = s.find_last_not_of(0x20);
	if(bpos < 0 || epos < 0)
		s.clear();
	else
		s = string(s, bpos, epos - bpos + 1);
}

void configref::insertItem(const config_item& item)
{
	vector<config_item>::iterator it_find
		= lower_bound(m_content.begin(), m_content.end(), item);

	if(it_find == m_content.end() || it_find->m_key != item.m_key)
	{
		SYS_LOG_DEBUG( "insert [" << item.m_key << "," << item.m_value << "]");
		m_content.insert(it_find, item);
	}
}

string configref::getString(const char* key, const std::string& default_value) const
{
	vector<config_item>::const_iterator it_find
		= lower_bound(m_content.begin(), m_content.end(), config_item(key, ""));

	if(it_find != m_content.end() && it_find->m_key == key)
	{
		SYS_LOG_DEBUG(key<<"="<<it_find->m_value);
		return it_find->m_value;
	} else {
		SYS_LOG_WARN("Config item '" << key << "' not found!! use default value "<<default_value);
		return default_value;
	}

	SYS_LOG_WARN("Config item '" << key << "' not found!!");
	return "";
}

int configref::getInt(const char *key, int default_value) const
{
	vector<config_item>::const_iterator it_find
		= lower_bound(m_content.begin(), m_content.end(), config_item(key, ""));

	if(it_find != m_content.end() && it_find->m_key == key)
	{
		SYS_LOG_DEBUG( key<<"="<<it_find->m_value);
		return atoi(it_find->m_value.c_str());
	} else {
		SYS_LOG_WARN( "Config item '" << key << "' not found!! use default value "<<default_value);
		return default_value;
	}

	SYS_LOG_WARN( "Config item '" << key << "' not found!!");
	return 0;
}

void configref::copyItemData(std::vector<config_item> &vecItem)
{
	vecItem.clear();

	vecItem.insert(vecItem.begin(), m_content.begin(), m_content.end());

	SYS_LOG_INFO("copy item data size:" << vecItem.size());
}


