#ifndef _LOGGER_H
#define _LOGGER_H

#ifdef _DEBUG
#include "zlog.h"
 
zlog_category_t *sys_logger;
#define LOG_INIT(path)            (zlog_init(path))
#define LOG_FINI()                zlog_fini();

#define IMPL_LOGGER(logger)          (logger = zlog_get_category(#logger))
#define DECL_LOGGER(logger)          extern zlog_category_t *logger;


#define SYS_LOG_INFO(...)       zlog_info(sys_logger, __VA_ARGS__)   
#define SYS_LOG_DEBUG(...)      zlog_debug(sys_logger, __VA_ARGS__)
#define SYS_LOG_ERROR(...)      zlog_error(sys_logger, __VA_ARGS__)
#define SYS_LOG_WARN(...)       zlog_warn(sys_logger, __VA_ARGS__)
#define SYS_LOG_FATAL(...)      zlog_fatal(sys_logger, __VA_ARGS__)

#else

#define LOG_INIT(path)    
#define LOG_FINI()       

#define IMPL_LOGGER(logger)      
#define DECL_LOGGER(logger)     

#define SYS_LOG_INFO(...)      
#define SYS_LOG_DEBUG(...)    
#define SYS_LOG_ERROR(...)   
#define SYS_LOG_WARN(...)   
#define SYS_LOG_FATAL(...) 

#endif /*end _DEBUG*/

#endif /*_LOGGER_H*/

