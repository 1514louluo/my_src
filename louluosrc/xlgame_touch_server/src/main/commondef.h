#pragma once
#include "stdint.h"
#include <map>
#include <string>

#ifndef __COMMONDEF__
#define __COMMONDEF__   1

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
// 
#define  GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION    "/boxserver/touchBoxaction"        
#define  GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG  "/boxserver/flushFilterConfig"  

//---------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------

//  
#define  GAMEBOX_SERVER_BOXSERVER_TOUCHBOXACTION_ID       0          
#define  GAMEBOX_SERVER_BOXSERVER_FLUSHFILTERCONFIG_ID       1     


enum enum_err_code
{
	SUCCESS=1, 
	FILT_FAIL_IN_RESTRICT_COND_PERIORD=2,  
	FILT_FAIL_IN_COMMON_COND_PERIORD=3,
	FILT_FAIL_IN_COMMON_BUSINESS_PERIORD=4,
	FILT_FAIL_IN_ENTRY_BUSINESS_PERIORD=5,
	FILT_FAIL_IN_FIXED_CONTENT_RESPONSE_PERIORD=6,
	FILT_FAIL_IN_COMMON_RESPONSE_PERIORD=7,
	FILT_FAIL_IN_COMMON_TASK_PERIORD=8,

	INTERNAL_TIMEOUT=9,
	PARAM_ERROR=10,
	FLUSH_FILTER_CONFIG_ERROR=11,
	FLUSH_UPDATE_TYPE_ERROR=12,

	ILLEGAL_ACCESS=13,
};

/*redis head*/
#define REDIS_HEAD_FOR_BUSINESS    					"10001"
#define REDIS_HEAD_FOR_UNRESTRICT_COND  			"10002"

#endif


