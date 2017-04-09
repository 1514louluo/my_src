#pragma once

#include "mycommon.h"
#include "SDUtility.h"
#include "SDLogger.h"

//#define  CONTENT_PATH_AUTH                     "auth"
//#define  CONTENT_PATH_CLI_QUERY_TRUSTEE        "cli.query.trustee"
//#define  CONTENT_PATH_GETKEY                   "getkey"
//#define  CONTENT_PATH_HEARTBEAT                "heartbeat"

#define  CONTENT_PATH_AUTH                     "/cli/notify/adapter/auth/post"
#define  CONTENT_PATH_AUTH_ACK                 "/svr/notify/adapter/auth/post"
#define  CONTENT_PATH_CLI_QUERY_TRUSTEE        "cli.query.trustee"
#define  CONTENT_PATH_GETKEY                   "/cli/notify/adapter/randomkey/get"
#define  CONTENT_PATH_GETKEY_ACK               "/svr/notify/adapter/randomkey/get"
#define  CONTENT_PATH_HEARTBEAT                "/cli/notify/adapter/heartbeat/post"
#define  CONTENT_PATH_HEARTBEAT_ACK            "/svr/notify/adapter/heartbeat/post"

#define  CONTENT_PATH_OFFLINE                  "/svr/notify/adapter/offline/post"


class message_codec
{
public:
	message_codec(void);
	~message_codec(void);

	void decode_content(std::string &content);
	std::string get_content_path(void);
	
	int content_auth_verify(void);

	int encode_auth(bool validation, std::string &content,std::string seqid,std::string tokenid );

	int encode_getkey_rtn(bool validation, std::string &randomkey,std::string &rtn_content);

	int encode_heartbeat_rtn(std::string &rtn_content);
	
	static void INIT_MESSAGE_CODEC(void);

	std::map<std::string, std::string> m_content_map;
private:
	DECL_LOGGER(logger);
	
	static std::string m_verify_key;

};
