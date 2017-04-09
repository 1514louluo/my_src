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
log4cplus::Logger g_task_opt_logger = log4cplus::Logger::getInstance("task_opt");
log4cplus::Logger g_socket_logger = log4cplus::Logger::getInstance("socket");
log4cplus::Logger g_userserver_logger = log4cplus::Logger::getInstance("userserver"); 
log4cplus::Logger g_logintrack_logger = log4cplus::Logger::getInstance("logintrack"); 
log4cplus::Logger g_mongodb_loger = log4cplus::Logger::getInstance("mongodb"); 
log4cplus::Logger g_3nd_service_loger =  log4cplus::Logger::getInstance("service_3nd"); 

