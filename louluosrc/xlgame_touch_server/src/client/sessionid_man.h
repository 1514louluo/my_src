#pragma once

#include "mycommon.h"
#include "SDLogger.h"
#include "SDUtility.h"
class sessionid_man
{
public:
	sessionid_man(void);
	~sessionid_man(void);

	static void INIT_SESSIONID(void);

	std::string create_sessionid(void);

private:
	MyMutex m_mutex;
	static uint64_t m_increseid;

	uint64_t get_increase(void);
	DECL_LOGGER(logger);
	
};
