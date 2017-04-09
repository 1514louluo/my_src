#pragma once

#include "SDLogger.h"
#include "commondef.h"
#include "mycommon.h"
#include "libcurlclient.h"
#include "md5.h"
#include "SDUtility.h"
class loginwebgame_service
{
public:
	loginwebgame_service(void);
	~loginwebgame_service(void);
	static int INIT_LOGINWEBGAME(void);
	/*int loginGame(std::string& username, std::string &password, std::string &userid, std::string &copartnerid,
		std::string &gameid,std::string &sid, std::string &loginurl, std::string &serverid, std::string &clientip,
		STU_GAME_RESULT *presult, int clientid,std::string comefrom, std::string startsource);*/

	/*std::string get_url_sign(std::string &username, std::string &password, std::string &userid, 
		std::string &copartnerid, std::string &gameid,
		std::string &sid, std::string &loginurl, std::string &serverid, std::string &clientip,
			int clientid,std::string comefrom,std::string startsource);*/

	std::string get_url_sign(std::string &username, std::string &password, std::string &userid, 
		std::string &copartnerid, std::string &gameid,
		std::string &sid, std::string &loginurl, std::string &serverid, std::string &clientip,
		int clientid,std::string comefrom,std::string comefrom1,
		std::string comefrom2,std::string comefrom3,std::string tokenid);

	int parseLoginGame(int weblogingame_rtn_code);

	int loginGame( std::string &username, std::string &password, std::string &userid, 
		std::string &copartnerid, std::string &gameid,std::string &sid, 
		std::string &loginurl, std::string &serverid, std::string &clientip, 
		STU_GAME_RESULT *presult , int clientid, std::string comefrom,
		std::string comefrom1,std::string comefrom2,std::string comefrom3,std::string tokenid);

	int loginGame_internal( std::string &username, std::string &password, std::string &userid, 
		std::string &copartnerid, std::string &gameid,std::string &sid, 
		std::string &loginurl, std::string &serverid, std::string &clientip, 
		STU_GAME_RESULT *presult , int clientid, std::string comefrom,
		std::string comefrom1,std::string comefrom2,std::string comefrom3,std::string logingameurl
		, std::string &gameLoginStatus,std::string tokenid);

private:
	static std::map<std::string, std::string> m_map_newgameid;
	static std::string m_old_gameserver;
	static std::string m_new_gameserver;
	static std::string m_new_gameserver_retry;
	static std::string m_md5key;
	static std::string m_gamelogin_rtn_default_errmsg;
	static int 		   m_timeout;
	DECL_LOGGER(logger);

	int decode_error_json(std::string &errstr, STU_GAME_RESULT *presult);

};
