#ifndef GAMEINFO_H_
#define GAMEINFO_H_
#include "mycommon.h"
#include "SDConfigurationSingleton.h"
#include "SDLogger.h"
#include "cmysql.h"
class gameinfo
{
	public:
		static gameinfo *GetInstance();		
		int get_gameinfo(std::string &, std::string &, std::string &, std::string &, std::string &);
		int get_gameinfo4special(std::string &, std::string &, std::string &, std::string &);
		int get_gameinfo_from_db(void);
		int get_gameinfo_from_db_4spe(void);
		~gameinfo();
	private:
		gameinfo();
		static gameinfo *instance;

		pthread_rwlock_t m_rwlock;
		pthread_rwlock_t m_spe_rwlock;
		std::map<std::string, std::string> m_gameinfo;	
		std::map<std::string, std::string> m_gameinfo_4spe;	
		
		DECL_LOGGER(logger);
};
#endif
