#ifndef _CONFIG_SINGLETON_H
#define _CONFIG_SINGLETON_H

#include "configref.h"

/* ʹ������Ϣȫ�ֻ����Ա������ط���Ҫʱ������*/
class config_singleton
{
public:
	~config_singleton(){delete m_config;}

	static config_singleton* get_instance(){return m_instance;}

	void init(configref* config);
	const configref& get_config() const;

protected:
	config_singleton(){m_config=NULL;}
	config_singleton(const config_singleton&){}
	config_singleton& operator=(const config_singleton&){return *this;}

private:
	static config_singleton* m_instance;

	configref* m_config;
};

#endif 

