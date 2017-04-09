#ifndef SD_CONFIGURATION_SINGLETON_H
#define SD_CONFIGURATION_SINGLETON_H

#include <common/SDConfiguration.h>

/* 使配置信息全局化，以便其它地方需要时即可用*/
class SDConfigurationSingleton
{
public:
	~SDConfigurationSingleton(){delete m_config;}

	static SDConfigurationSingleton* get_instance(){return m_instance;}

	void init(SDConfiguration* config);
	const SDConfiguration& get_config() const;

protected:
	SDConfigurationSingleton(){m_config=NULL;}
	SDConfigurationSingleton(const SDConfigurationSingleton&){}
	SDConfigurationSingleton& operator=(const SDConfigurationSingleton&){return *this;}

private:
	static SDConfigurationSingleton* m_instance;

	SDConfiguration* m_config;
};

#endif /* end SD_CONFIGURATION_SINGLETON_H */

