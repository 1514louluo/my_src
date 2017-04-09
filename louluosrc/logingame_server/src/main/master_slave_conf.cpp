#include "master_slave_conf.h"
#include "SDConfigurationSingleton.h"

master_slave_conf *master_slave_conf::m_pthis=NULL;
master_slave_conf::master_slave_conf(void)
{
}

master_slave_conf::~master_slave_conf(void)
{
}

master_slave_conf * master_slave_conf::singleton( void )
{
	if (m_pthis == NULL)
	{
		m_pthis = new master_slave_conf;
		const SDConfiguration& config = SDConfigurationSingleton::get_instance()->get_config();
		std::string master_slave;
		master_slave = config.getString("service_ms_type","master");
		if (master_slave == "master")
		{
			m_pthis->m_ismaster=true;
		} 
		else
		{
			m_pthis->m_ismaster=false;
		}
	}

	return m_pthis;

}

bool master_slave_conf::is_master( void )
{
	return m_ismaster;

}
