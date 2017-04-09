#include "global_var.h"

global_var::global_var(void)
{
}

global_var::~global_var(void)
{
}



log4cplus::Logger g_stat_logger = log4cplus::Logger::getInstance("stat");
log4cplus::Logger g_perform_logger = log4cplus::Logger::getInstance("perform");
log4cplus::Logger g_service_logger = log4cplus::Logger::getInstance("service");
//log4cplus::Logger g_logingame_logger = log4cplus::Logger::getInstance("logingame");
//log4cplus::Logger g_loginbox_logger = log4cplus::Logger::getInstance("loginbox");
//log4cplus::Logger g_bindtourist_logger = log4cplus::Logger::getInstance("bindtourist");
log4cplus::Logger g_socket_logger = log4cplus::Logger::getInstance("socket");
//log4cplus::Logger g_gamelib_logger = log4cplus::Logger::getInstance("gamelib");
//log4cplus::Logger g_userserver_logger = log4cplus::Logger::getInstance("userserver"); 
//log4cplus::Logger g_sysmsg_logger = log4cplus::Logger::getInstance("sysmsg"); 
//log4cplus::Logger g_logintrack_logger = log4cplus::Logger::getInstance("logintrack"); 
log4cplus::Logger g_mongodb_loger = log4cplus::Logger::getInstance("mongodb"); 

log4cplus::Logger g_activity_loger = log4cplus::Logger::getInstance("activity"); 
log4cplus::Logger g_platform_loger = log4cplus::Logger::getInstance("platform"); 
