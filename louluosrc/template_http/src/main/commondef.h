#pragma once
#include "stdint.h"
#include <map>
#include <string>

typedef struct _STU_ZMQ_INFO_IDENTITY
{
	std::string identity;
	std::string cmd_type;  // heartbeat  data
	std::string routing_destination; 
	std::string data;
}STU_ZMQ_INFO_IDENTITY;


typedef struct _STU_ZMQ_INFO
{
	std::string cmd_type;  // heartbeat  data
	std::string destination; 
	std::string data;
}STU_ZMQ_INFO;


//#define  ZMQ_CMD_TYPE_REG       "reg"
#define	 ZMQ_CMD_TYPE_HEARTBEAT "heartbeat"
#define  ZMQ_CMD_TYPE_DATA      "data"
#define  ZMQ_CMD_TYPE_CONFIG    "config"


#define  ZMQ_ERROR   zmq_strerror(zmq_errno())

#ifndef __COMMONDEF__
#define __COMMONDEF__   1

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
// 登陆注册协议集
 
#define  GAMEBOX_SERVER_PROPERTY_POST                              "/notify/property_server/all_property/post"      
#define  GAMEBOX_SERVER_PROPERTY_DELETE                          "/notify/property_server/all_property/delete"   
#define  GAMEBOX_SERVER_PROPERTY_QUERY                            "/notify/property_server/all_property/query"      
#define  GAMEBOX_SERVER_PROPERTY_QUERYBATCH                 "/notify/property_server/all_property/queryBatch"      
        
//---------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------

// 登陆注册协议集       
#define GAMEBOX_SERVER_PROPERTY_POST_ID                              0
#define GAMEBOX_SERVER_PROPERTY_DELETE_ID                           1
#define GAMEBOX_SERVER_PROPERTY_QUERY_ID                            2
#define GAMEBOX_SERVER_PROPERTY_QUERYBATCH_ID                 3

enum enum_err_code
{
	SUCCESS=1, 
	INTERNAL_TIMEOUT=2,
	PARAM_ERROR=3,
	GET_REDIS_FROM_POOL_ERROR=4,

	ILLEGAL_ACCESS=5,
};
/*redis head*/
#define REDIS_GAMEINTERNALNO_HEAD                    "10"
#define REDIS_UID_HEAD                                           "20"
#define REDIS_CLIENTTYPE_HEAD                              "30"
#define REDIS_VERSION_HEAD                                   "40"
#define REDIS_COMEFROM_HEAD                                "50"
#define REDIS_PID_HEAD                                            "60"
#endif


