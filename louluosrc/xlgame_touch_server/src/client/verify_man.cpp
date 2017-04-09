#include "verify_man.h"

uint64_t verify_man::m_increseid=0;

IMPL_LOGGER(verify_man, logger);
verify_man::verify_man(void)
{
}

verify_man::~verify_man(void)
{
}

void verify_man::INIT_SESSIONID( void )
{
	srandom(time(NULL));
	m_increseid = random();
}

std::string verify_man::create_verifykey( void )
{
	

	std::string sessionid;
	while (sessionid.size()<32)
	{
		uint64_t randomid1 = random();
		uint64_t increaseid1 = get_increase();
		std::string str_random1 = SDUtility::format(randomid1);
		std::string str_increase1 = SDUtility::format(increaseid1);
		int hash1 = SDUtility::hash_gcid(str_random1);
		int hash2 = SDUtility::hash_gcid(str_increase1);
		if (hash1<0)
		{
			hash1 = 0-hash1;
		}
		if (hash2<0)
		{
			hash2 = 0-hash2;
		}

		std::string str_hash1 = SDUtility::format(hash1);
		std::string str_hash2 = SDUtility::format(hash2);

		sessionid = sessionid + str_hash1 + str_hash2;

	}
	if (sessionid.size() == 32)
	{
		return sessionid;
	} 
	else
	{
		return sessionid.substr(0,32);
	}
	




}

uint64_t verify_man::get_increase( void )
{
	scopedlock mylock(m_mutex.get());
	if (m_increseid == 0xFFFFFFFFFFFFFFF0)
	{
		m_increseid = 0xFF;
	}
	m_increseid++;
	return m_increseid;

}
