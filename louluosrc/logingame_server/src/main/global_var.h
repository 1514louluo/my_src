#pragma once

#include "SDLogger.h"

class global_var
{
public:
	global_var(void);
	~global_var(void);
};




extern log4cplus::Logger g_stat_logger;
extern log4cplus::Logger g_perform_logger;
extern log4cplus::Logger g_service_logger;
extern log4cplus::Logger g_logingame_logger;
extern log4cplus::Logger g_loginbox_logger;
extern log4cplus::Logger g_bindtourist_logger;
extern log4cplus::Logger g_socket_logger;
extern log4cplus::Logger g_gamelib_logger;
extern log4cplus::Logger g_userserver_logger;
extern log4cplus::Logger g_sysmsg_logger;
extern log4cplus::Logger g_logintrack_logger;
extern log4cplus::Logger g_mongodb_loger;

#define  STAT_LOG(logEvent)		LOG4CPLUS_INFO(g_stat_logger, logEvent)  
#define  PERFORM_LOG(logEvent)  LOG4CPLUS_INFO(g_perform_logger, logEvent)  
#define  SERVICE_LOG(logEvent)  LOG4CPLUS_INFO(g_service_logger, logEvent) 
#define  LOGINGAME_LOG(logEvent)  LOG4CPLUS_INFO(g_logingame_logger, logEvent) 
#define  LOGINBOX_LOG(logEvent)  LOG4CPLUS_INFO(g_loginbox_logger, logEvent) 
#define  BIND_LOG(logEvent)     LOG4CPLUS_INFO(g_bindtourist_logger, logEvent) 
#define  SOCK_ERR_LOG(logEvent)     LOG4CPLUS_ERROR(g_socket_logger, logEvent) 
#define  GAMELIB_LOG(logEvent)     LOG4CPLUS_ERROR(g_gamelib_logger, logEvent) 
#define  USERSERVER_LOG(logEvent)     LOG4CPLUS_ERROR(g_userserver_logger, logEvent) 
#define  SYSMSG_LOG(logEvent)     LOG4CPLUS_ERROR(g_sysmsg_logger, logEvent) 
#define  LOGINTRACK_LOG(logEvent)     LOG4CPLUS_INFO(g_logintrack_logger, logEvent) 
#define  MONGODB_LOG(logEvent)     LOG4CPLUS_ERROR(g_mongodb_loger, logEvent) 


