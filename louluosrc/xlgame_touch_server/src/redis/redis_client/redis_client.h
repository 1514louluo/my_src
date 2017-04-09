#pragma once

#include "hiredis/hiredis.h"
#include "mycommon.h"
#include "SDLogger.h"
#include "SDConfigurationSingleton.h"
#include "SDUtility.h"
#include "boost/algorithm/string.hpp"

#define REDIS_REPLY_STATUS_OK    "OK"
/*<====================================>*/
#define REDIS_CMD_EXPIRE         "EXPIRE"
#define REDIS_CMD_GET            "GET"
#define REDIS_CMD_SET            "SET"
#define REDIS_CMD_SETEX        "SETEX"
#define REDIS_CMD_DEL            "DEL"
#define REDIS_CMD_SADD           "SADD"
#define REDIS_CMD_SMEMBER        "SMEMBERS"
#define REDIS_CMD_SREM			 "SREM"
#define REDIS_CMD_SISMEMBER 			 "SISMEMBER"

#define REDIS_CMD_HSET           "HSET"
#define REDIS_CMD_HGET		 "HGET"
#define REDIS_CMD_HGETALL        "HGETALL"
#define REDIS_CMD_HDEL           "HDEL"

#define REDIS_CMD_INCRBY             "INCRBY"
#define REDIS_CMD_DECRBY             "DECRBY"
/*<====================================>*/

class redis_client
{
public:
	redis_client(const string_type&, int, int, int);
	~redis_client(void);
	int connect(void);

/*pipe ÷ª÷ß≥÷–¥√¸¡Ó*/
//	int pipeline_wtcmd(const string_pair_vector &cmd, string_pair_vector &out);
	
	int set(const string_type &str_key, const string_type &value);
       int setex(const string_type &str_key, const string_type &value, unsigned int secs);
	int get(const string_type &key, string_type &value);
	int del(const string_type &key);
	int expire(const string_type & key, unsigned int secs);
	
	int hget(const string_type &key,const string_type &field, string_type &value);
	int hgetall(const string_type &key, string_pair_vector &out);
	int hset(const string_type &key, const string_type &field, const string_type &value);
	int hdel(const string_type &key, const string_type &field);

	int incrby(const string_type &key, const string_type &value);
	int decrby(const string_type &key, const string_type &value);

	int sadd(const string_type &key, const string_type &value);
	int sismember(const string_type &key, const string_type &value);
private:
	redisContext  *m_redis_context;
	string_type   m_redis_ip;
	int           m_redis_port;
	int           m_timeout_seconds;
	int           m_expire_seconds;
	std::map<string_type, int>           m_type_map;

	bool is_cmd_send_out(void);
	int commit_cmd(redisReply **ppreply);
	int check_connection(void);

	DECL_LOGGER(logger);
};
