#pragma once



#define   MACRO_CHECK_MASTER_SLAVE(operationname)   if(master_slave_conf::singleton()->is_master() == false)  \
									{ \
										MY_LOGDEBUG("server in slave, drop operation="<<operationname);  \
										return 0;}

class master_slave_conf
{
public:
	static master_slave_conf *singleton(void);
	
	bool is_master(void);

private:
	static master_slave_conf *m_pthis;
	master_slave_conf(void);
	~master_slave_conf(void);
	bool m_ismaster;
};
