#include "SDConfigurationSingleton.h"

using namespace std;

SDConfigurationSingleton* SDConfigurationSingleton::m_instance = new SDConfigurationSingleton();

void SDConfigurationSingleton::init(SDConfiguration* config)
{
	m_config = config;
}

const SDConfiguration& SDConfigurationSingleton::get_config() const
{
	return (*m_config);
}

