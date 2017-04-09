

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

#define  PATH_CTL_CMD_ADDQUEUE       "/xlmq/addqueue"
#define  PATH_CTL_CMD_DELQUEUE       "/xlmq/delqueue"
#define  PATH_CTL_CMD_LISTQUEUE       "/xlmq/listqueue"
#define  PATH_CTL_CMD_STATQUEUE		 "/xlmq/statqueue"

#define  PATH_CTL_CMD_ADDQUEUE_ID      0
#define  PATH_CTL_CMD_DELQUEUE_ID      1
#define  PATH_CTL_CMD_LISTQUEUE_ID     2
#define  PATH_CTL_CMD_STATQUEUE_ID     3

#define RC_OK_ID           "1"
#define RC_ERR_ID          "2"

#define RC_OK_INFO         "success"
#define RC_ERR_INFO         "error"

