#include "sessionid_man.h"

uint64_t sessionid_man::m_increseid=0;

IMPL_LOGGER(sessionid_man, logger);
sessionid_man::sessionid_man(void)
{
}

sessionid_man::~sessionid_man(void)
{
}

void sessionid_man::INIT_SESSIONID( void )
{
	srandom(time(NULL));
	m_increseid = random();
}

std::string sessionid_man::create_sessionid( void )
{
	// 一共32位数
	/*uint64_t randomid1 = random();
	uint64_t randomid2 = random();
	uint64_t increaseid1 = get_increase();
	uint64_t increaseid2 = get_increase();

	std::string str_random1 = SDUtility::format(randomid1);
	std::string str_random2 = SDUtility::format(randomid2);
	std::string str_increase1 = SDUtility::format(increaseid1);
	std::string str_increase2 = SDUtility::format(increaseid2);

	std::string hash1 = SDUtility::hash_gcid(str_random1);
	std::string hash2 = SDUtility::hash_gcid(str_random2);
	std::string hash3 = SDUtility::hash_gcid(str_increase1);
	std::string hash4 = SDUtility::hash_gcid(str_increase2);

	MY_LOGDEBUG("hash1="<<hash1<<" hash2="<<hash2<<" hash3="<<hash3<<" hash4="<<hash4);

	return hash1.substr(0,8) + hash2.substr(0,8) + hash3.substr(0,8) + hash4.substr(0,8);*/

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

uint64_t sessionid_man::get_increase( void )
{
	scopedlock mylock(m_mutex.get());
	if (m_increseid == 0xFFFFFFFFFFFFFFF0)
	{
		m_increseid = 0xFF;
	}
	m_increseid++;
	return m_increseid;

}
