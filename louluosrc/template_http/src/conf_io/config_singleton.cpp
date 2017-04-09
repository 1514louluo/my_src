#include "config_singleton.h"

using namespace std;

config_singleton* config_singleton::m_instance = new config_singleton();

void config_singleton::init(configref* config)
{
	m_config = config;
}

const configref& config_singleton::get_config() const
{
	return (*m_config);
}

