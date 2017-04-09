#pragma once

#include "hiredis/hiredis.h"
#include "mycommon.h"
#include "global_var.h"
#include "SDLogger.h"
#include "SDConfigurationSingleton.h"
#include "SDUtility.h"
#include "boost/algorithm/string.hpp"


#define REDIS_REPLY_STATUS_OK    "OK"

#define REDIS_CMD_EXPIRE         "EXPIRE"
#define REDIS_CMD_GET            "GET"
#define REDIS_CMD_SET            "SET"
#define REDIS_CMD_DEL            "DEL"
#define REDIS_CMD_SADD           "SADD"
#define REDIS_CMD_SMEMBER        "SMEMBERS"
#define REDIS_CMD_SREM			 "SREM"


class redis_client
{
public:
	redis_client(void);
	~redis_client(void);

	static void INIT_REDIS_CLIENT(void);

	int set_value(std::string &str_key, std::string &str_value);

	int get_value(std::string &str_key, std::string &str_value);
	
	int set_expire(std::string &str_key,int timeout_second=0);
	int set_value_set(std::string &str_key, std::string &str_value);
	int get_value_set(std::string &str_key, std::vector<std::string> &vec);
	int del_value_set(std::string &str_key, std::string &str_value);

private:
	redisContext * m_redis_context;
	static std::string m_redis_ip;
	static int         m_redis_port;
	static int         m_timeout_seconds;
	static int         m_expire_seconds;

	bool is_cmd_send_out(void);

	int commit_cmd(redisReply **ppreply);

	int check_connection(void);

	DECL_LOGGER(logger);
};
